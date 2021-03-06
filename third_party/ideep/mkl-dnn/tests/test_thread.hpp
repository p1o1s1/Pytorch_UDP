/*******************************************************************************
* Copyright 2020-2022 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#ifndef TEST_THREAD_HPP
#define TEST_THREAD_HPP

#if DNNL_GRAPH_CPU_RUNTIME == DNNL_GRAPH_RUNTIME_THREADPOOL
#include "oneapi/dnnl/dnnl_graph_threadpool_iface.hpp"
namespace dnnl {
namespace graph {

// Original threadpool utils are used by the scoped_tp_activation_t and thus
// need to be re-declared because of the hack above.
namespace impl {
namespace threadpool_utils {
void activate_threadpool(dnnl::graph::threadpool_interop::threadpool_iface *tp);
void deactivate_threadpool();
dnnl::graph::threadpool_interop::threadpool_iface *get_active_threadpool();
} // namespace threadpool_utils
} // namespace impl

namespace testing {

dnnl::graph::threadpool_interop::threadpool_iface *get_threadpool();

// Sets the testing threadpool as active for the lifetime of the object.
// Required for the tests that throw to work.
struct scoped_tp_activation_t {
    scoped_tp_activation_t() {
        impl::threadpool_utils::activate_threadpool(get_threadpool());
    }
    ~scoped_tp_activation_t() {
        impl::threadpool_utils::deactivate_threadpool();
    }
};

} // namespace testing
} // namespace graph
} // namespace dnnl
#endif

#endif
