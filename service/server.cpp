#include <nodepp/nodepp.h>
#include <express/https.h>
#include <nodepp/wss.h>

using namespace nodepp;

void onMain() {

    auto app = express::https::add();

    app.ALL( [=]( express_https_t cli ){
        cli.header( "Cross-Origin-Opener-Policy"  , "same-origin"  );
        cli.header( "Cross-Origin-Embedder-Policy", "require-corp" );
    });

    app.USE( express::https::file( "./view" ) );

    app.listen( "0.0.0.0", 8000, []( ssocket_t ){
        console::log( "-> https://localhost:8000 ->", dns::get_hostname() );
    } );

}