window.addEventListener( "load", ()=>{

    Engine().then(( ev )=>{ window.engine = ev;

        ev.canvas = document.getElementById('canvas');

        document.querySelector( "button" ).addEventListener( "click",()=>{
            window.document.querySelector("canvas").requestFullscreen();
            setTimeout(()=>{ ev.refresh(); },1);
        });

        setTimeout(()=>{ start_ar_controller(); },1000);

    }).catch(( err )=>{
        console.log( err );
        alert( "error while launching the game" );
    });

});
