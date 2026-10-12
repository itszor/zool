#!/bin/sh
# Build Zool from your own copy of the game:
#     ./build.sh Zool          (the ArcFS archive the release came as, or a .zip holding it)
# then: cd recomp && ./zool
# The steps are recipe.json's "build", run by SuperSpider's tools/cook.py.
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
SUPERSPIDER="${SUPERSPIDER:-$HOME/stuff/superspider}"
[ $# -gt 0 ] || { echo "usage: $0 ZOOL-ARCHIVE | GAME.zip" >&2; exit 2; }
(cd "$SUPERSPIDER" && dune build)
COOK_RECIPES="$HERE" exec python3 -I "$SUPERSPIDER/tools/cook.py" build "$@"
