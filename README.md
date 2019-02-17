# heckel-diff

This is a simple tool that can be called by *git-diff* to detect line moves.  By default it outputs the number of added and removed lines, similarly to the output of `git diff --numstat`, but does not report moved lines as added or removed.  It can also output an (unsorted) list of lines, similar to the output of plain `git diff` by setting the environment variable `HECKEL_DIFF_LINES`.

The project is mostly based on the [C++ library](https://github.com/rowungiles/heckel_diff) developed by Rowun Giles.  According to him, this was inspired by IGListKit (https://github.com/Instagram/IGListKit), and based on Paul Heckel's linear time diffing paper (http://documents.scribd.com/docs/10ro9oowpo1h81pgh1as.pdf).

## Use
- `cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Release && cd build && make install`
- `GIT_EXTERNAL_DIFF=heckel-diff-stat git diff ...`

## Modify
The `Algorithm<T>` currently accepts `std::string`, `size_t` and `uint32_t`
- `vi heckel-diff-stat/heckel-diff-stat.cpp` (or favourite editor) and do your modifications

### Notes
The tests have a wall_clock and cpu_clock (`TEST(HeckelDiff, Benchmark)`) test set to expect 1600 diffs to run in no greater than wall_clock 16.67ms (60fps). You may have to adjust this as your computer requires.

The project uses some of the non NSIndexPath test conditions from IGListKit (https://github.com/Instagram/IGListKit).

CPPLint.cfg is available.
