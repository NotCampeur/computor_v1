FILE="computor_v1"
OUTPUT_FILE="tests/output.log"

if test -f "$FILE"; then
	echo "File "$FILE" exists, proceeding to unit tests"
else
	echo "File "$FILE" does not exist, aborting"
	exit 1
fi

./"$FILE" "42 = 42" > "$OUTPUT_FILE" 2>&1
./"$FILE" "42" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "5^" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "5^six" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "quatre egal 4" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "6.480740698^2 = 42" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "54e2 = 0" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "42.000000000000000000000000000000000000002 = 42" >> "$OUTPUT_FILE" 2>&1
./"$FILE" "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0" >> "$OUTPUT_FILE" 2>&1

diff tests/output.log tests/expected_output.log