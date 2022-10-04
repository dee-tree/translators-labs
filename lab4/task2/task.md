# Theme 3. Task 2

---
Calculate list of words (excluding too short words which are shorter than 4 characters).
* This list must be sorted by alphabet order.
* This list must remove duplicates.

Use **sort** and **uniq** utilities.

Before this, do:
* Write *lex* program to get words that longer than 3 chars and outputs them.
* Lex program's output must be given as sort input: `test < 1.in | sort`

---

Run tests: `./x.sh`
Run from stdin: `./words </dev/stdin | sort -u` or just `./sorted_words.sh`