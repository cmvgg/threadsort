#!/bin/bash

echo "Running configuration tests..."
bash test_config.sh
echo ""

echo "Running command-line argument tests..."
bash test_cli_args.sh
echo ""

echo "Running signal tests (automated)..."
bash test_signals.sh
echo ""

echo "All tests have finished. Review the results in the logs_sh directory."