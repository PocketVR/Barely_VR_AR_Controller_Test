#include <nodepp/nodepp.h>
#include <ungine/ungine.h>
#include <nodepp/json.h>
#include <nodepp/bind.h>

using namespace nodepp;

void resize ( int width, int height ){ ungine::window::resize({ width, height }); }

void refresh( /*-----------------*/ ){
    auto width  = EM_ASM_INT({ return window.screen.width ; });
    auto height = EM_ASM_INT({ return window.screen.height; });
    resize( width, height ); /*-----------------------------*/
}

BIND( Engine ){
BIND_ADD( "resize" , &resize  )
BIND_ADD( "refresh", &refresh )
}

namespace ungine { void run() {

    node::node_vr_scene([=]( ref_t<node_t> self ){

        self->append_child( "camera", node::fly_camera_3D([=]( ref_t<node_t> self ){

            auto pos = self->get_attribute<transform_3D_t>( "transform" );
            auto cam = self->get_attribute<camera_3D_t>   ( "camera" );
                 pos->position.y = 15; cursor::lock();

        }) );

        self->append_child( node::node_3D([=]( ref_t<node_t> self ){

            model_t obj ( "./controller/assets/scene_0.glb" );
            auto    shader = shader_t();

            shader.set_fragment_shader( GPU_KERNEL(

                // Input vertex attributes (from vertex shader)
                varying vec2 fragTexCoord;
                varying vec4 fragColor;

                // Input uniform values
                uniform sampler2D texture0;
                uniform vec4 colDiffuse;

                // NOTE: Add your custom variables here

                void main(){
                    // Texel color fetching from texture sampler
                    vec4 texelColor = texture2D(texture0, fragTexCoord);

                    // NOTE: Implement here your fragment shader code

                    gl_FragColor = texelColor*colDiffuse;
                }

            ) ); 
        
        /*
            console::log( "->", obj->materialCount );
            obj->materials[0].shader = shader.get();
        */

            auto pos = self->get_attribute<transform_3D_t>( "transform" );
                 pos->scale *= 10;

            self->on3DDraw([=](){ shader.emit([=](){
                obj.draw( *pos ); 
            }); });

        }) );

        self->on2DDraw([=](){ rl::DrawFPS ( 30, 30  ); });
        self->on3DDraw([=](){ rl::DrawGrid( 10, 100 ); });

    });

}}

void onMain() {

    auto width  = EM_ASM_INT({ return window.screen.width;  });
    auto height = EM_ASM_INT({ return window.screen.height; });

    EM_ASM({
        window.addEventListener( "resize", ()=>{ engine.refresh(); });
        window.sensor = new Object({ x:"0", y:"0", z:"0", w:"0" });
    });

//  ungine::rl::SetConfigFlags( ungine::rl::FLAG_MSAA_4X_HINT );
    ungine::engine::start( width, height, "GAME" );
    ungine::rl::rlSetClipPlanes( 0.5, 200 );
    ungine::engine::set_fps( 60 );
    ungine::run();

}
