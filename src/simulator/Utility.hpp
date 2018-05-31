#pragma once

#include <vector>
#include <iostream>
#include <cassert>

#include "Machine.hpp"

namespace utility
{
namespace machines
{
std::vector<MachineSP> readFromStream(std::istream& inputStream);
std::vector<MachineSP> generate(unsigned machinesNum, unsigned capacity);
}

namespace algorithm
{
using SrcRow = unsigned;
using DstRow = unsigned;
using SrcRowOffset = unsigned;
using DstRowOffset = unsigned;

template <class TObject>
void deterministic_swap(
    std::vector<std::vector<TObject> >& table,
    SrcRow& srcRow,
    DstRow& dstRow,
    SrcRowOffset& srcRowOffset,
    DstRowOffset& dstRowOffset)
{
  std::swap(table[srcRow][srcRowOffset], table[dstRow][dstRowOffset]);
}

template <class TObject>
std::tuple<SrcRow, DstRow, SrcRowOffset, DstRowOffset> random_swap(
    std::vector<std::vector<TObject> >& table)
{
  unsigned srcRow = rand() % table.size();
  while (table[srcRow].size() <= 0)
    srcRow = (srcRow + 1) % table.size();
  unsigned dstRow = rand() % table.size();
  while (table[dstRow].size() <= 0)
    dstRow = (dstRow + 1) % table.size();

  unsigned srcRowOffset = rand() % table[srcRow].size();
  unsigned dstRowOffset = rand() % table[dstRow].size();

  deterministic_swap<TObject>(table, srcRow, dstRow, srcRowOffset, dstRowOffset);

  return std::make_tuple(srcRow, dstRow, srcRowOffset, dstRowOffset);
}

template <class TObject>
void deterministic_move(
    std::vector<std::vector<TObject> >& table,
    SrcRow& srcRow,
    DstRow& dstRow,
    SrcRowOffset& srcRowOffset,
    DstRowOffset& dstRowOffset)
{
  if(srcRow == dstRow)
    return;

  table[dstRow].insert(table[dstRow].begin() + dstRowOffset,
                       table[srcRow][srcRowOffset]);
  table[srcRow].erase(table[srcRow].begin() + srcRowOffset);
}

template <class TObject>
std::tuple<SrcRow, DstRow, SrcRowOffset, DstRowOffset> random_move(
    std::vector<std::vector<TObject> >& table)
{
  unsigned srcRow = rand() % table.size();
  while (table[srcRow].size() <= 0)
    srcRow = (srcRow + 1) % table.size();
  unsigned dstRow = rand() % table.size();
  if (dstRow == srcRow)
    dstRow = (dstRow + 1) % table.size();

  unsigned srcRowOffset = rand() % table[srcRow].size();
  unsigned dstRowOffset = rand() % (table[dstRow].size() + 1);

  deterministic_move<TObject>(table, srcRow, dstRow, srcRowOffset, dstRowOffset);

  return std::make_tuple(srcRow, dstRow, srcRowOffset, dstRowOffset);
}
}
}
