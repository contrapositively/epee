#include "man.hpp"

#include <ctfile>

using namespace csheet;

const std::string sheet_manager::PATH = "sheets";
const std::string SHEET_META_FILE = sheet_manager::PATH + ".meta";
typedef ctfl::cexpr_file<
    sheet_manager::sheet_list_t
> meta_file_t;

meta_file_t meta_file(SHEET_META_FILE);
