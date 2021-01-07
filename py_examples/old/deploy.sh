#!/bin/bash

printf "Deploy lamdbda"

rm lambda.zip
zip lambda.zip lambda.py helpers/
aws lambda update-function-code --function-name motion-analysis --zip-file fileb://lambda.zip