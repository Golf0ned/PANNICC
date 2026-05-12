#include <tao/pegtl.hpp>

namespace pegtl = tao::pegtl;

namespace frontend {

template <typename Rule> using pegtl_try = pegtl::seq<pegtl::at<Rule>, Rule>;

}
