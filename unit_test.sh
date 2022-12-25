FILE="computor_v1"

if test -f "$FILE"; then
	echo "File "$FILE" exists, proceeding to unit tests"
else
	echo "File "$FILE" does not exist, aborting"
	exit 1
fi

./"$FILE" "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0"
