#include "options.hpp"

using namespace opts;



bool add_flag(const options::option_id id)
{};
options::flag_t &get_flag(const options::option_id id)
{};

bool add_option(const options::option_id id)
{};
options::option_t &get_option(const options::option_id id)
{};

bool add_option_list(const options::option_id id)
{};
options::option_list_t &get_option_list(const options::option_id id)
{};

bool add_long_alias(const options::option_id id, const std::string_view alias)
{};
bool add_short_alias(const options::option_id id, const char alias)
{};

options::options()
{};
options::~options()
{};
