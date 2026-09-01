# wince-extra (shelved COREDLL import surfaces)

Files moved here are the COREDLL import surfaces for Windows CE
generations **outside the supported 4.x / 5.x / 6.x scope**.  They are
preserved for reference (git history, sync pairs with
w32api/wince-extra) but are **not built**: the mingw32ce `LIBS` list
in `../Makefile.in` builds only

- `libcoredll.a`  (CE 5.0 / Windows Mobile 6, `coredll.def`)
- `libcoredll4.a` (CE 4.x, `coredll4.def`)
- `libcoredll6.a` (CE 6.0, `coredll6.def`)

| File | Generation | Notes |
|---|---|---|
| `coredll3.def` | Windows CE 3.0 | Pocket PC 2000-augmented |
| `coredll7.def` | Windows Embedded Compact 7 | includes the InputScope exports |
| `coredll8.def` | Windows Embedded Compact 2013 | includes the InputScope exports |
| `out_coredll7.def` | Compact 7 | earlier dump kept for reference |
| `out_coredll8.def` | Compact 2013 | earlier dump kept for reference |

The 4.x/5.x/6.x surfaces were verified symbol-for-symbol against real
device SDKs (metarutaiga WINCE400/WINCE600 coredll import surfaces).

If a generation outside 4.x/5.x/6.x is ever needed again: move the
matching `.def` back to the repo root and re-add `libcoredllN.a` to
`LIBS` and the dependency list in `../Makefile.in`.
