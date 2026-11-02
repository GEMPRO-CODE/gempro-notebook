# gempro-notebook
Materiais impressos para a maratona de programação.

`gempro.pdf` contém os templates mais importantes para estágios mais avançados da ICPC.
`gempro-extended.pdf` contém todos os templates importantes, especialmente para a primeira fase da maratona.

## Compilação

Execute `bash build.sh` na raiz para gerar os dois PDFs, ou use `bash build.sh core`
e `bash build.sh extended` para compilar apenas uma versão. São necessários
LaTeX com minted, Python 3, `cpp` e Clang com libc++.

O script prepara os snippets uma única vez em `out/prepared/`, reutilizando
`out/template-cache/`, e executa duas passagens de LaTeX por versão. Mantenha
`out/` entre compilações para aproveitar os caches dos snippets e do minted.
Alterações no conteúdo dos snippets invalidam seu cache automaticamente; após
alterar a lógica de geração em `scripts/prepare.py`, remova `out/template-cache/`
para regenerar os resultados. A compilação direta com `pdflatex` continua
funcionando, mas prepara cada snippet separadamente e é mais lenta.

A preparação usa até quatro processos de compilação simultâneos para acelerar
compilações sem cache. Ajuste com `GEMPRO_JOBS=2 bash build.sh` em máquinas com
pouca memória, ou `GEMPRO_JOBS=8 bash build.sh` para experimentar mais paralelismo.
Use `GEMPRO_JOBS=1 bash build.sh` para execução sequencial. Também é possível
preparar apenas os snippets com `python3 scripts/prepare.py --all --jobs 4`.

Execute `python3 -m unittest discover -s tests` para testar a preparação em lote
e a reutilização do cache.
