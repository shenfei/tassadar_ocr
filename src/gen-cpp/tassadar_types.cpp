/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "tassadar_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>



int _kPageSegModeValues[] = {
  PageSegMode::PSM_OSD_ONLY,
  PageSegMode::PSM_AUTO_OSD,
  PageSegMode::PSM_AUTO_ONLY,
  PageSegMode::PSM_AUTO,
  PageSegMode::PSM_SINGLE_COLUMN,
  PageSegMode::PSM_SINGLE_BLOCK_VERT_TEXT,
  PageSegMode::PSM_SINGLE_BLOCK,
  PageSegMode::PSM_SINGLE_LINE,
  PageSegMode::PSM_SINGLE_WORD,
  PageSegMode::PSM_CIRCLE_WORD,
  PageSegMode::PSM_SINGLE_CHAR,
  PageSegMode::PSM_SPARSE_TEXT,
  PageSegMode::PSM_SPARSE_TEXT_OSD,
  PageSegMode::PSM_COUNT
};
const char* _kPageSegModeNames[] = {
  "PSM_OSD_ONLY",
  "PSM_AUTO_OSD",
  "PSM_AUTO_ONLY",
  "PSM_AUTO",
  "PSM_SINGLE_COLUMN",
  "PSM_SINGLE_BLOCK_VERT_TEXT",
  "PSM_SINGLE_BLOCK",
  "PSM_SINGLE_LINE",
  "PSM_SINGLE_WORD",
  "PSM_CIRCLE_WORD",
  "PSM_SINGLE_CHAR",
  "PSM_SPARSE_TEXT",
  "PSM_SPARSE_TEXT_OSD",
  "PSM_COUNT"
};
const std::map<int, const char*> _PageSegMode_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(14, _kPageSegModeValues, _kPageSegModeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

std::ostream& operator<<(std::ostream& out, const PageSegMode::type& val) {
  std::map<int, const char*>::const_iterator it = _PageSegMode_VALUES_TO_NAMES.find(val);
  if (it != _PageSegMode_VALUES_TO_NAMES.end()) {
    out << it->second;
  } else {
    out << static_cast<int>(val);
  }
  return out;
}


