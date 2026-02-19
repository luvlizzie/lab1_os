#!/bin/bash

echo "=== Running Employee Records System Tests ==="
echo "Version 1.0"
echo "=============================================="

rm -rf build
mkdir build
cd build

cmake ..
make

CREATOR_INPUT_1="vik\n50\njayce\n48\nsky\n32"
CREATOR_INPUT_2="vik\n50"
CREATOR_INPUT_3="vik\n50\njayce\n48\nsky\n32\nekko\n40\npowder\n40"

echo -e "\n=== Running All Tests ===\n"

echo ">>> CreatorTest.CreatesBinaryFile"
echo -e "$CREATOR_INPUT_1" | ./test_runner --gtest_filter=CreatorTest.CreatesBinaryFile
echo ""

echo ">>> CreatorTest.WritesCorrectEmployeeData"
echo -e "$CREATOR_INPUT_2" | ./test_runner --gtest_filter=CreatorTest.WritesCorrectEmployeeData
echo ""

echo ">>> CreatorTest.CreatesMultipleEmployees"
echo -e "$CREATOR_INPUT_3" | ./test_runner --gtest_filter=CreatorTest.CreatesMultipleEmployees
echo ""

echo ">>> ReporterTest.CreatesReportFile"
./test_runner --gtest_filter=ReporterTest.CreatesReportFile
echo ""

echo ">>> ReporterTest.ReportHasCorrectContent"
./test_runner --gtest_filter=ReporterTest.ReportHasCorrectContent
echo ""

echo ">>> ReporterTest.HandlesEmptyFile"
./test_runner --gtest_filter=ReporterTest.HandlesEmptyFile
echo ""

echo ">>> ReporterTest.CalculatesCorrectSalaries"
./test_runner --gtest_filter=ReporterTest.CalculatesCorrectSalaries
echo ""

echo "=============================================="
echo "✓ All tests completed"
echo "=============================================="

cd ..
