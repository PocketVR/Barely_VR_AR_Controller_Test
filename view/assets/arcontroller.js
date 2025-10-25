function ar_loop( arController, video ) { try {
    for( var i in window.marker ){ window.marker.visible=false; }
    arController.process( video ); /*--------------------------*/
    requestAnimationFrame(()=>{ ar_loop( arController, video ); });
} catch(err) { alert( err ); }}

function ar_setup( arController, video ) {

    arController.setPatternDetectionMode( artoolkit.AR_MATRIX_CODE_DETECTION );

    arController.addEventListener( 'getMarker', (ev)=>{
    if( ev.data.marker.idMatrix === -1 ){ return; }
        engine.marker( ev.data.marker.idMatrix, ev.data.matrix );
    }); ar_loop( arController, video );

}

function start_ar_controller() {

    if( navigator.mediaDevices && navigator.mediaDevices.getUserMedia ) {

        var hint = new Object({
            facingMode: { "ideal": "environment" },
            audio     : false
        });

        var video = document.querySelector("video");

        navigator.mediaDevices.getUserMedia({video:hint}).then((stream)=>{
            video.srcObject  = stream; video.play();
            var arController = new ARController( video, "/assets/camera_para.dat" );
                arController.onload = ()=>{ ar_setup( arController, video ); }
        }).catch((err)=>{
            console.log( err );
            alert( "error while stating camera " );
        });

    } else { alert( "camera not found" ); }

}
