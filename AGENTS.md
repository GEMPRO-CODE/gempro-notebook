# Repository Guidelines

## Project Structure & Module Organization

This repository produces printed competitive-programming notebooks in Portuguese. Algorithm snippets live under `content/{dsa,geometry,graph,math,strings,tree}/`; each topic's `main.tex` supplies descriptions and includes snippets. `content/problems/` holds extended-only subproblems, `content/contest/` contest scaffolding, and `content/extra/` reference material.

`content/core.tex` and `content/extended.tex` select variants, sharing `book.tex` and `preamble.tex`. `scripts/prepare.py` generates hashed listings. Root PDFs are deliverables; ignored `out/` holds build artifacts and caches. Keep compact within 25 pages.

## Build, Test, and Development Commands

Run commands from the repository root. Install a LaTeX distribution with the packages used by `content/preamble.tex`, minted's highlighting dependencies, Python 3, `cpp`, and Clang with libc++ headers.

- `bash build.sh` (or `bash build.sh all`): build both variants and refresh `gempro.pdf` and `gempro-extended.pdf`.
- `bash build.sh core`: build only the compact notebook.
- `bash build.sh extended`: build only the expanded notebook.
- `bash build.sh legacy`: build the legacy entry point into `out/gempro-notebook.pdf`.
- `clang++ -std=c++20 -stdlib=libc++ -fsyntax-only content/graph/dijkstra.cpp`: syntax-check an individual snippet.

Each PDF build runs LaTeX twice. Builds enable shell escape to invoke the preparation script; run them only on trusted source.

## Coding Style & Naming Conventions

Use C++20, matching `compile_flags.txt`. Preserve neighboring code's compact contest style, tab indentation, aliases, and macros. Use lowercase, hyphenated snippet filenames such as `directed-mst.cpp`. Python tooling uses four-space indentation. No formatter or linter is configured.

Preserve the exact `// begin template //` and `// end template //` markers. Add notebook entries in the topic's `main.tex`, documenting assumptions, inputs, outputs, and complexity in Portuguese; follow existing core/extended conditionals.

## Notebook Formatting

Keep styles in `content/preamble.tex`; do not override heading sizes, caption spacing, or listing padding by topic. Use `\templateentry` for snippets, `\mintedentry` for literal files, and `\notebookentry` for custom entries. Descriptions follow bold inline titles in both editions; empty descriptions keep standalone titles. Category headings remain standalone. Use one heading per algorithm; reserve subsubsections for actual children such as FFT/NTT under convolution.

Use sentence case, preserving proper names and acronyms. Heading arguments have no final period; inline entry punctuation is automatic. Bold theorem labels end with a period. Write code identifiers in `\texttt{}` in prose and `\mathtt{}` in math; keep mathematical variables italic. Omit visible spaces after commas inside code fragments; permit wrapping with `\allowbreak` after commas in long signatures. Use “construção” and “consulta” in prose, and `0-based`/`1-based` for indexing; preserve API names. Prefer concise contracts over implementation walkthroughs.

## Writing Descriptions & Reference Notes

Write for experienced ICPC contestants consulting the notebook under time pressure. Optimize useful information per printed space. For each sentence, ask: would removing it plausibly cause misuse or meaningful time spent deciphering the snippet? If not, remove it. Apply this standard to both editions; extended space permits useful adaptations, not extra narration.

- Keep non-obvious input requirements, output meanings, interval/index conventions, mutation, composition order, sentinels, and time/memory bounds. Preserve amortized/expected qualifications and algorithm-specific hazards such as transform limits, geometric degeneracies, coordinate bounds, and sentinel collisions.
- Omit implementation walkthroughs, standard method inventories, long signatures visible immediately below, obvious dependencies, and restatements of titles or formulas. For example, omit “use Gaussian elimination for the determinant” and “`count(x)` counts occurrences”; retain “`kth` returns the XOR value, not the stored element.”
- State each fact once. Consolidate shared conventions, such as the general arithmetic reminder and geometry's “Reais” marker, while retaining local exceptions. Avoid duplicating recipes between snippets and reference notes in the same edition; check both variants before removing a duplicate.
- Keep plain-language explanations of unfamiliar results. Do not replace meaningful descriptions with unexplained notation merely to shorten them. Reference notes should retain hypotheses, definitions, and actionable constructions; omit redundant derivations, proof sketches, generic motivation, and immediate special cases.
- Shorten precisely: replace vague qualifications with explicit conditions, and preserve distinctions such as exact versus bounded weight, independent-insertion witness IDs, or ordered noncommutative aggregation. Length alone is not a reason to cut an essential contract.

Treat the debugging checklist differently: familiar mistakes are intentional prompts that help authors question their own code. Preserve its organized reminders. Keep required attribution, and edit generators alongside generated prose (for example, `scripts/divisor_bounds.py`). Review rendered space savings and readability rather than source word count alone.

## Testing Guidelines

Run relevant Python unittest suites, e.g. `python3 -m unittest discover -s tests -p test_problems.py -v`. There is no coverage threshold. For algorithm changes, use boundary cases and randomized comparisons against brute force. For prose/layout changes, rebuild both PDFs and inspect pagination, wrapping, and overflow. Record validation in the PR.

## Commit & Pull Request Guidelines

History uses short, lowercase subjects such as `tree distance histogram` and `shorten directed mst`; no mandatory prefix is evident. Prefer specific summaries and focused commits. PRs should explain the change, list validation performed, link relevant issues, and include page screenshots for layout changes. Review regenerated PDF changes before including them.
