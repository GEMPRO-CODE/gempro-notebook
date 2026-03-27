#!/bin/bash

set -e

mkdir -p out

build_pdf() {
	local jobname="$1"
	local source="$2"
	pdflatex -shell-escape --output-directory=./out/ --jobname="$jobname" "$source"
	pdflatex -shell-escape --output-directory=./out/ --jobname="$jobname" "$source"
}

case "${1:-all}" in
	core)
		build_pdf gempro content/core.tex
		cp out/gempro.pdf .
		;;
	extended)
		build_pdf gempro-extended content/extended.tex
		cp out/gempro-extended.pdf .
		;;
	all)
		build_pdf gempro content/core.tex
		cp out/gempro.pdf .
		build_pdf gempro-extended content/extended.tex
		cp out/gempro-extended.pdf .
		;;
	legacy)
		build_pdf gempro-notebook content/main.tex
		;;
	*)
		echo "usage: $0 [all|core|extended|legacy]" >&2
		exit 1
		;;
esac
