#!/usr/bin/env python3
"""
Collects per-run Accelerate metrics and writes metrics/run_metrics.json.
Called by CI at the end of each pipeline run (success or failure).

Required environment variables (set by GitHub Actions):
  COMMIT_TIMESTAMP, PIPELINE_START, JOB_STATUS,
  GITHUB_RUN_ID, GITHUB_RUN_NUMBER, GITHUB_SHA
"""

import datetime
import json
import os


def main():
    context = read_env_context()
    metrics = build_metrics(context)
    write_metrics(metrics)
    print(json.dumps(metrics, indent=2))


def read_env_context():
    return {
        'commit_timestamp': os.environ.get('COMMIT_TIMESTAMP', ''),
        'pipeline_start':   os.environ.get('PIPELINE_START', ''),
        'status':           os.environ.get('JOB_STATUS', 'unknown'),
        'run_id':           os.environ.get('GITHUB_RUN_ID', ''),
        'run_number':       int(os.environ.get('GITHUB_RUN_NUMBER', '0')),
        'commit_sha':       os.environ.get('GITHUB_SHA', ''),
    }


def build_metrics(ctx):
    now       = datetime.datetime.now(datetime.timezone.utc)
    commit_ts = parse_ts(ctx['commit_timestamp'])
    return {
        'run_id':             ctx['run_id'],
        'run_number':         ctx['run_number'],
        'commit_sha':         ctx['commit_sha'],
        'commit_timestamp':   ctx['commit_timestamp'],
        'pipeline_start':     ctx['pipeline_start'],
        'pipeline_end':       now.strftime('%Y-%m-%dT%H:%M:%SZ'),
        'lead_time_seconds':  compute_lead_time(commit_ts, now),
        'status':             ctx['status'],
        'components_built':   detect_built_components(),
        'components_skipped': detect_skipped_components(),
    }


def write_metrics(metrics):
    os.makedirs('metrics', exist_ok=True)
    with open('metrics/run_metrics.json', 'w') as f:
        json.dump(metrics, f, indent=2)


def parse_ts(s):
    if not s:
        return None
    return datetime.datetime.fromisoformat(s.replace('Z', '+00:00'))


def compute_lead_time(commit_ts, now):
    if not commit_ts:
        return None
    return int((now - commit_ts).total_seconds())


def detect_built_components():
    return [c for c in all_components() if component_was_built(c)]


def detect_skipped_components():
    return [c for c in all_components() if not component_was_built(c)]


def all_components():
    return ['computer', 'pico', 'pi']


def component_was_built(name):
    if name == 'computer':
        return os.path.exists('computer/webserver/webserv/webserv')
    return os.path.exists(f'{name}/CMakeLists.txt')


if __name__ == '__main__':
    main()
