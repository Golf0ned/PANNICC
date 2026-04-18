#include <tao/pegtl.hpp>

namespace pegtl = tao::pegtl;
using namespace pegtl;

template <typename Rule> using pegtl_try = pegtl::seq<pegtl::at<Rule>, Rule>;
