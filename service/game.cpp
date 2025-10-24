#include <nodepp/nodepp.h>
#include <ungine/ungine.h>
#include <nodepp/json.h>
#include <nodepp/bind.h>

using namespace nodepp;

object_t marker_list;

void resize ( int width, int height ){ ungine::window::resize({ width, height }); }

void refresh( /*-----------------*/ ){
    auto width  = EM_ASM_INT({ return window.screen.width ; });
    auto height = EM_ASM_INT({ return window.screen.height; });
    resize( width, height ); /*-----------------------------*/
}

void marker ( const int& id, const EM_VAL& data ){

    auto matrix = ungine::mat_t({
        data[0].as<float>(), data[4].as<float>(), data[8] .as<float>(), data[12].as<float>(),
        data[1].as<float>(), data[5].as<float>(), data[9] .as<float>(), data[13].as<float>(),
        data[2].as<float>(), data[6].as<float>(), data[10].as<float>(), data[14].as<float>(),
        data[3].as<float>(), data[7].as<float>(), data[11].as<float>(), data[15].as<float>()
    });

    marker_list["0"] = object_t({ // string::to_string(id)
        { "visible", true   },
        { "data"   , matrix }
    });

}

BIND( Engine ){
BIND_ADD( "marker" , &marker  )
BIND_ADD( "resize" , &resize  )
BIND_ADD( "refresh", &refresh )
}

namespace ungine { vec4_t get_sensor_data() { try {

    char res[UNBFF_SIZE]; auto size = EM_ASM_INT({
        var data = JSON.stringify( window.sensor );
        stringToUTF8( data, $0, $1 ); return data.length;
    }, res, UNBFF_SIZE );

    auto data = json::parse( string_t( res, size ) );

    return vec4_t({
        string::to_float( data["x"].as<string_t>() ),
        string::to_float( data["y"].as<string_t>() ),
        string::to_float( data["z"].as<string_t>() ),
        string::to_float( data["w"].as<string_t>() )
    });

} catch( except_t ) { return vec4_t({ 0, 0, 0, 0 }); }} }

namespace ungine { void run() {

    node::node_vr_scene([=]( ref_t<node_t> self ){

        self->append_child( "camera", node::camera_3D([=]( ref_t<node_t> self ){

            auto pos = self->get_attribute<transform_3D_t>( "transform" );
            auto cam = self->get_attribute<camera_3D_t>   ( "camera" );
                 pos->position.y = 10 ; // cursor::lock();

            self->onLoop([=]( float delta ){

                auto mot      = get_sensor_data();
                auto rot /**/ = math::quaternion::multiply(
                    math::quaternion::from_axis({ { 1, 0 , 0 }, PI/2 }), 
                    vec4_t({ mot.y, mot.x,-mot.z, mot.w })
                );

                pos->rotation = math::quaternion::to_euler( rot );

            });

            self->append_child( node::node_3D([=]( ref_t<node_t> self ){

                auto pos = self->get_attribute<transform_3D_t>( "transform" );
                auto obj = model_t( "./controller/assets/obj.glb" );

                self->onLoop([=]( float ){
                    for( auto x: marker_list.keys() ){ auto y = marker_list[x];
                    if ( !y["visible"].as<bool>  () ){ continue; }

                        auto mat = y["data"].as<mat_t>();

                        pos->scale    = vec3_t({ 1, 1, 1 });

                        pos->rotation = math::matrix::to_euler( mat )
                                      * vec3_t({ -1, -1, 1 });

                        pos->position = vec3_t({-mat.m12,-mat.m13, mat.m14 })
                                      * vec3_t({ 1.2, 1.2, 0.8 });

                    y["visible"] = false; }
                });

                self->on3DDraw([=](){ obj.draw( *pos ); });

            }) );

        }) );

        self->append_child( node::node_3D([=]( ref_t<node_t> self ){

            model_t obj ( "./controller/assets/scene_0.glb" );

            auto pos = self->get_attribute<transform_3D_t>( "transform" );
                 pos->scale *= 10;

            self->on3DDraw([=](){ obj.draw( *pos ); });

        }) );

        self->on2DDraw([=](){ rl::DrawFPS ( 30, 30  ); });
        self->on3DDraw([=](){ rl::DrawGrid( 10, 100 ); });

    });

}}

void onMain() {

    auto width  = EM_ASM_INT({ return window.screen.width;  });
    auto height = EM_ASM_INT({ return window.screen.height; });

    EM_ASM({ (()=>{
        window.addEventListener( "resize", ()=>{ engine.refresh(); });
        window.sensor = new Object({ x:"0", y:"0", z:"0", w:"0" });
    try {
        const  sensor = new AbsoluteOrientationSensor({ frequency: 60 });

        sensor.addEventListener( "reading", ()=>{
            window.sensor.x = sensor.quaternion[0].toFixed(6);
            window.sensor.y = sensor.quaternion[1].toFixed(6);
            window.sensor.z = sensor.quaternion[2].toFixed(6);
            window.sensor.w = sensor.quaternion[3].toFixed(6);
        });

        sensor.start();
    } catch(err) { alert( "gyroscope not found" ); } })(); });

//  ungine::rl::SetConfigFlags( ungine::rl::FLAG_MSAA_4X_HINT );
    ungine::engine::start( width, height, "GAME" );
    ungine::rl::rlSetClipPlanes( 0.5, 200 );
    ungine::engine::set_fps( 60 );
    ungine::run();

}
