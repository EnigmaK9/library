#!/bin/bash
# Generates library.txt listing each file’s full path and its contents,
# separated by a line with a single dash.

output="library.txt"

# (Re)create/empty the output file
> "$output"

for file in *; do
  if [ -f "$file" ]; then
    # Print full path
    echo "$(pwd)/$file" >> "$output"
    # Separator
    echo "-" >> "$output"
    # Print file contents
    cat "$file" >> "$output"
    # Separator before next file
    echo "-" >> "$output"
  fi
done

