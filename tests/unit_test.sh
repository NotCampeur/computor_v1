FILE="computor_v1"
OUTPUT_FILE="tests/output.log"

if test -f "$FILE"; then
	echo "File "$FILE" exists, proceeding to unit tests"
else
	echo "File "$FILE" does not exist, aborting"
	exit 1
fi

./"$FILE" "42 = 42" > "$OUTPUT_FILE" 2>&1
./"$FILE" "42 = 52 - 20 + 10" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "52 - 20 + 10 = 42" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "42" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "42^0 = 1" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "10^1.5 = 31.6227766" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "5^" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "5^six" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "quatre egal 4" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "6.480740698^2 = 42" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "54e2 = 0" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "42.000000000000000000000000000000000000002 = 42" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "42 * X^0 = 42 * X^0" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "42 * X^1 = 42 * X^1" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "5 + 4X - 9.3X^2 = 1" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 5 * X^0 + 4 * X^1 - 9.3 * X^2" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "5 * X^0 + 4 * X^1 = 4 * X^0" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "4X + 5 = 6X" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "4 * X^29 = 4 * X^29" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "4X*-5X" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "4X*-5X - 8X^2" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "2 * 15 * X = -7 * 4" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "2^3 * X^2 + 4 * 2X * X + 6 * 4" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "4 * 3X8 = 0" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "5^-2 = 0.04" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "X^4 * X^-2 + X + 14 = 17" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "X^2 + X + 14 + X^-2 = 17" >> "$OUTPUT_FILE" 2>&1

diff --expand-tabs --ignore-tab-expansion --side-by-side --left-column tests/output.log tests/expected_output.log > tests/diff.log
echo "tests/diff.log created, please check it for differences between expected and actual output"