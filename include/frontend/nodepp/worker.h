/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_WORKER
#define NODEPP_WORKER

/*────────────────────────────────────────────────────────────────────────────*/
#include "wasm/worker.cpp"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace worker { 

    template< class V, class... T >
    void await( V cb, const T&... args ){ 
        worker_t wrk( cb, args... ); wrk.await();
    }
    
    template< class V, class... T >
    worker_t add( V cb, const T&... args ){ 
        worker_t wrk( cb, args... ); 
        wrk.add(); return wrk; 
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
