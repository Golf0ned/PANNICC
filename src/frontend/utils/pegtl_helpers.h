#include <tao/pegtl.hpp>

namespace frontend {
    template <typename Rule>
    using pegtl_try = tao::pegtl::seq<tao::pegtl::at<Rule>, Rule>;
}
