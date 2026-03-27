I want to create a GitHub Actions CI/CD workflow that not only builds and tests my project, but also collects metrics based on the four Accelerate metrics: 

1. Lead time: time between commit and successful build/test.
2. Deployment frequency: number of pipeline runs per period (day/week).
3. Time to recover: time between failed pipeline and next successful pipeline.
4. Change failure rate: percentage of failed pipeline runs.

Requirements:

- The workflow should run on Ubuntu latest.
- Steps: checkout, build, run tests, collect metrics.
- Metrics should be stored in a JSON file as artifacts.
- Include timestamps to compute lead time and TTR.
- Provide an example JSON structure for metrics.
- Ensure the JSON artifact can be downloaded and accessed via GitHub API by an external script (like the PC monitoring dashboard).
- Include optional step to generate a simple chart or table for visualization.

Please generate the full GitHub Actions YAML workflow for this setup.
