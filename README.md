## Skimmer

The code skims the branches of the input root file such that the output root file has only the required branches.

## Submitting Condor Jobs

### Check the settings before submitting the job
```python3 submitCondorJobs.py --jobname <jobname> --codedir <path to VLLAna.C> --dryrun yes```

### Submit one job for testing
```python3 submitCondorJobs.py --jobname <jobname> --codedir <path to VLLAna.C> --test yes```

### Submit Condor Jobs
```python3 submitCondorJobs.py --jobname <jobname> --codedir <path to VLLAna.C>```

