#!/usr/bin/env python3
"""
Computes cross-run Accelerate metrics from GitHub Actions artifacts.

Downloads all 'accelerate-metrics-*' artifacts for the repo, then computes:
  - Lead time for changes (average seconds from commit to green pipeline)
  - Deployment frequency (successful runs per day over the period)
  - Time to restore (average seconds from first failure to next success)
  - Change failure rate (failed runs / total runs)

Usage:
  export GITHUB_TOKEN=ghp_...
  export GITHUB_REPOSITORY=owner/repo   # e.g. docteurbadluck/3D_scanner
  python3 metrics/aggregate.py [--output metrics/summary.json] [--days 30]

Output:
  Prints a summary table and writes summary.json (or --output path).
"""

import argparse
import json
import os
import sys
import urllib.request
import urllib.error
import datetime
import zipfile
import io


# ── GitHub API helpers ────────────────────────────────────────────────────────

def test_authentication(token, repo):
    url = f"https://api.github.com/repos/{repo}/actions/artifacts"
    req = urllib.request.Request(url, headers={
        'Authorization': f'Bearer {token}',
        'Accept': 'application/vnd.github+json',
    })
    

def gh_get(url, token):
    req = urllib.request.Request(url, headers={
        'Authorization': f'Bearer {token}',
        'Accept': 'application/vnd.github+json',
        'X-GitHub-Api-Version': '2022-11-28',
    })
    try:
        with urllib.request.urlopen(req) as resp:
            return json.loads(resp.read())
    except urllib.error.HTTPError as e:
        error_message = e.read().decode('utf-8')  # Decode the error response
        print(f"Error {e.code}: {e.reason} - {error_message}")
        # If GitHub returns the WWW-Authenticate header, print it
        www_authenticate = e.headers.get('WWW-Authenticate', 'No WWW-Authenticate header found.')
        print(f"WWW-Authenticate header: {www_authenticate}")
        raise

def _resolve_artifact_url(url, token):
    class NoRedirect(urllib.request.HTTPRedirectHandler):
        def http_error_302(self, req, fp, code, msg, headers):
            raise urllib.error.HTTPError(req.full_url, code, msg, headers, fp)
        http_error_301 = http_error_303 = http_error_307 = http_error_302
    req = urllib.request.Request(url, headers={
        'Authorization': f'Bearer {token}',
        'Accept': 'application/vnd.github+json',
    })
    opener = urllib.request.build_opener(NoRedirect)
    try:
        opener.open(req).close()
        return url
    except urllib.error.HTTPError as e:
        if 'Location' not in e.headers:
            raise
        return e.headers['Location']


def download_bytes(url, token):
    signed_url = _resolve_artifact_url(url, token)
    with urllib.request.urlopen(signed_url) as resp:
        return resp.read()


def fetch_all_run_metrics(repo, token, days):
    """Return list of parsed run_metrics dicts from all matching artifacts."""
    cutoff = datetime.datetime.now(datetime.timezone.utc) - datetime.timedelta(days=days)
    runs = []
    page = 1

    while True:
        url = (
            f'https://api.github.com/repos/{repo}/actions/artifacts'
            f'?per_page=100&page={page}'
        )
        data = gh_get(url, token)
        artifacts = data.get('artifacts', [])
        if not artifacts:
            break

        for art in artifacts:
            if not art['name'].startswith('accelerate-metrics-'):
                continue
            created = datetime.datetime.fromisoformat(
                art['created_at'].replace('Z', '+00:00')
            )
            if created < cutoff:
                continue

            # Download zip and extract run_metrics.json
            try:
                raw = download_bytes(art['archive_download_url'], token)
                with zipfile.ZipFile(io.BytesIO(raw)) as zf:
                    with zf.open('run_metrics.json') as jf:
                        metrics = json.loads(jf.read())
                        runs.append(metrics)
            except Exception as exc:
                print(f'  Warning: could not read artifact {art["name"]}: {exc}',
                      file=sys.stderr)

        if len(artifacts) < 100:
            break
        page += 1

    return runs


# ── Accelerate metric computations ───────────────────────────────────────────

def parse_ts(s):
    if not s:
        return None
    return datetime.datetime.fromisoformat(s.replace('Z', '+00:00'))


def compute_metrics(runs):
    if not runs:
        return None

    runs_sorted = sorted(runs, key=lambda r: r.get('pipeline_end', ''))

    total   = len(runs_sorted)
    success = [r for r in runs_sorted if r.get('status') == 'success']
    failure = [r for r in runs_sorted if r.get('status') == 'failure']

    # Lead time: average seconds from commit to pipeline end (successes only)
    lead_times = [r['lead_time_seconds'] for r in success
                  if r.get('lead_time_seconds') is not None]
    avg_lead_time = sum(lead_times) / len(lead_times) if lead_times else None

    # Deployment frequency: successful runs per day over the observed period
    dates = [parse_ts(r['pipeline_end']) for r in success if r.get('pipeline_end')]
    if len(dates) >= 2:
        span_days = (max(dates) - min(dates)).total_seconds() / 86400
        deploy_freq = len(success) / span_days if span_days > 0 else None
    elif len(dates) == 1:
        deploy_freq = 1.0  # Only one run, frequency undefined but not zero
    else:
        deploy_freq = None

    # Time to restore: for each failure, find the next success and measure gap
    ttr_values = []
    for fail in failure:
        fail_end = parse_ts(fail.get('pipeline_end'))
        if not fail_end:
            continue
        next_success = next(
            (parse_ts(r['pipeline_end']) for r in runs_sorted
             if r.get('status') == 'success'
             and parse_ts(r.get('pipeline_end', '')) > fail_end),
            None
        )
        if next_success:
            ttr_values.append((next_success - fail_end).total_seconds())

    avg_ttr = sum(ttr_values) / len(ttr_values) if ttr_values else None

    # Change failure rate
    cfr = len(failure) / total if total > 0 else 0.0

    return {
        'period_days':              None,   # filled by caller
        'total_runs':               total,
        'successful_runs':          len(success),
        'failed_runs':              len(failure),
        'lead_time_avg_seconds':    avg_lead_time,
        'lead_time_avg_minutes':    round(avg_lead_time / 60, 1) if avg_lead_time else None,
        'deployment_frequency_per_day': round(deploy_freq, 3) if deploy_freq else None,
        'time_to_restore_avg_seconds':  avg_ttr,
        'time_to_restore_avg_minutes':  round(avg_ttr / 60, 1) if avg_ttr else None,
        'change_failure_rate':      round(cfr, 4),
        'change_failure_rate_pct':  round(cfr * 100, 2),
        'computed_at':              datetime.datetime.now(datetime.timezone.utc)
                                        .strftime('%Y-%m-%dT%H:%M:%SZ'),
    }


# ── Markdown table ────────────────────────────────────────────────────────────

def print_table(summary):
    rows = [
        ('Metric',                         'Value'),
        ('─' * 35,                         '─' * 20),
        ('Lead time (avg)',                 f"{summary['lead_time_avg_minutes']} min"
                                            if summary['lead_time_avg_minutes'] else 'n/a'),
        ('Deployment frequency',            f"{summary['deployment_frequency_per_day']} / day"
                                            if summary['deployment_frequency_per_day'] else 'n/a'),
        ('Time to restore (avg)',           f"{summary['time_to_restore_avg_minutes']} min"
                                            if summary['time_to_restore_avg_minutes'] else 'n/a'),
        ('Change failure rate',             f"{summary['change_failure_rate_pct']} %"),
        ('Total runs',                      str(summary['total_runs'])),
        ('Successful / Failed',             f"{summary['successful_runs']} / {summary['failed_runs']}"),
    ]
    for label, value in rows:
        print(f'  {label:<35} {value}')


# ── Main ──────────────────────────────────────────────────────────────────────

WEB_OUTPUT_DEFAULT = 'computer/webserver/www/data/productivity.json'


def main():
    args  = parse_args()
    token = read_env_or_exit('GITHUB_TOKEN')
    repo  = read_env_or_exit('GITHUB_REPOSITORY')

    runs = fetch_runs(repo, token, args.days)
    if not runs:
        print('No data to aggregate.')
        return

    summary = build_summary(runs, args.days)
    print_summary(summary)
    write_json(summary, args.output)
    write_json(summary, args.output_web)


def parse_args():
    parser = argparse.ArgumentParser(description='Aggregate Accelerate metrics from CI artifacts.')
    parser.add_argument('--output',     default='metrics/summary.json')
    parser.add_argument('--output-web', default=WEB_OUTPUT_DEFAULT)
    parser.add_argument('--days', type=int, default=30)
    return parser.parse_args()


def read_env_or_exit(name):
    val = os.environ.get(name)
    if not val:
        sys.exit(f'Error: {name} environment variable is not set.')
    return val


def fetch_runs(repo, token, days):
    print(f'Fetching artifacts for {repo} (last {days} days)...')
    runs = fetch_all_run_metrics(repo, token, days)
    print(f'  Found {len(runs)} run(s).')
    return runs


def build_summary(runs, days):
    summary = compute_metrics(runs)
    summary['period_days'] = days
    return summary


def print_summary(summary):
    print()
    print('Accelerate Metrics Summary')
    print('=' * 57)
    print_table(summary)
    print()


def write_json(data, path):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'w') as f:
        json.dump(data, f, indent=2)
    print(f'Written to {path}')


if __name__ == '__main__':
    main()
