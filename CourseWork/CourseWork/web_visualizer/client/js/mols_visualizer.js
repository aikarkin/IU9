var scene, camera, light, root, renderer;

var windowHalfX, windowHalfY;
var mouseX, mouseY;
var deltaX, deltaY;
var mouseDown = false;

var touchX, touchY;
var touchDown = false;
var deltaTouchX, deltaTouchY;

var touchDist;
var deltaTouchDist;

var vdw_rad_enabled = false;
var rad_c0;

var MAX_SCALE = 15;
var MIN_SCALE = 1;
var cur_scale = 2;

var menu = document.getElementById( "menu" );
var cur_id = 1;

var MOLECULES = {
    1: "Water",
    2: "Ethylene chloride",
    3: "Caffeine",
    4: "Chloroform"
}

function getBoundingBoxCenter() {
    var root_bb = new THREE.Box3();
    root_bb.setFromObject(root);
    var root_c = root_bb.getCenter();

    return root_c;
}

function addStick(start, end, color) {
    var boxGeometry = new THREE.BoxBufferGeometry( 1, 1, 1 );
    var s = new THREE.Vector3(start.x, start.y, start.z);
    var e = new THREE.Vector3(end.x, end.y, end.z);

    var b_color = new THREE.Color();
    b_color.r = color[0];
    b_color.g = color[1];
    b_color.b = color[2];

    var b_object = new THREE.Mesh( boxGeometry, new THREE.MeshPhongMaterial( {color: b_color} ) );

    s.multiplyScalar( 20 );
    e.multiplyScalar( 20 );

    b_object.position.copy( s );
    b_object.position.lerp( e, 0.5 );
    b_object.scale.set( 2, 2, s.distanceTo( e ) );
    b_object.lookAt( e );

    root.add(b_object);
}

function render() {
    requestAnimationFrame(render);
    renderer.render(scene, camera);
}

function init() {
    scene = new THREE.Scene();

    windowHalfX = window.innerWidth / 2;
	windowHalfY = window.innerHeight / 2;

    mouseDown = false;
    mouseX = 0;
    mouseY = 0;
    deltaX = 0;
    deltaY = 0;

    touchDown = false;
    touchX = 0;
    touchY = 0;
    touch2X = 0;
    touch2Y = 0;
    deltaTouchX = 0;
    deltaTouchY = 0;

    touchDist = 0;
    deltaTouchDist = 0;

	camera = new THREE.PerspectiveCamera( 70, window.innerWidth / window.innerHeight, 1, 5000 );
	camera.position.z = 1000;
	scene.add( camera );

	var light = new THREE.DirectionalLight( 0xffffff, 0.8 );
	light.position.set( 1, 1, 1 );
	scene.add(light);

	var light = new THREE.DirectionalLight( 0xffffff, 0.5 );
	light.position.set( -1, -1, 1 );
	scene.add(light);

	root = new THREE.Group();
	scene.add(root)

    root.scale.x = cur_scale;
    root.scale.y = cur_scale;
    root.scale.z = cur_scale;

    //root.position.set(-200, -200, -400);

	renderer = new THREE.WebGLRenderer( { antialias: true } );
	renderer.setClearColor( 0x050505 );
	renderer.setPixelRatio( window.devicePixelRatio );
	renderer.setSize( window.innerWidth, window.innerHeight );
	document.getElementById( 'container' ).appendChild( renderer.domElement );

    document.addEventListener('mousemove', onMouseMove, false);
    document.addEventListener('mouseup', onMouseUp, false);
    document.addEventListener('mousedown', onMouseDown, false);
    document.addEventListener('keydown', onKeyDown, false);
    document.addEventListener('mousewheel', onMouseWheel, false);

    document.addEventListener("touchstart", onTouchStart, {passive: false});
    document.addEventListener("touchmove", onTouchMove, {passive: false});
    //document.addEventListener("touchend", onTouchEnd, false);

    window.addEventListener( 'resize', onWindowResize, false );
}

/*  coord: THREE.Vector3
    color: THREE.Color
    radius: number  */
function addAtom(position, color, radius) {
    var sphereGeometry = new THREE.IcosahedronBufferGeometry(radius, 2);
    sphereGeometry.dynamic = true;

    var material = new THREE.MeshPhongMaterial( { color: color } );
    var atomMesh = new THREE.Mesh( sphereGeometry, material );

    atomMesh.name = 'atom';
    atomMesh.position.copy(position);
    atomMesh.position.multiplyScalar( 20 );
	atomMesh.scale.multiplyScalar( 20);

    root.add(atomMesh);
}

// mol deserialized from json
function addMol(mol, rad_c=0.3) {
    // console.log(mol);
    var atoms = mol["atoms"];
    rad_c0 = rad_c;

    for(var a_key in atoms) {
        var atom = atoms[a_key];

        var color = new THREE.Color(atom['color'][0], atom['color'][1], atom['color'][2])
        var pos = new THREE.Vector3(atom['position'][0], atom['position'][1], atom['position'][2])
        var rad = atom['vdw_radius']*rad_c;

        addAtom(pos, color, rad);
    }

    if(rad_c != 1.0) {
        var bonds = mol["bonds"];

        for(var b of bonds) {
            var b_idx, e_idx;
            var keys = Object.keys(b);
            b_idx = keys[0];
            e_idx = b[b_idx];


            var a_b = atoms[b_idx];
            var a_e = atoms[e_idx];

            var start = new THREE.Vector3();
            var end = new THREE.Vector3();

            start.x = a_b['position'][0];
            start.y = a_b['position'][1];
            start.z = a_b['position'][2];

            end.x = a_e['position'][0];
            end.y = a_e['position'][1];
            end.z = a_e['position'][2];

            var dir = new THREE.Vector3(end.x - start.x, end.y - start.y, end.z - start.z);
            var bond_len = dir.length();
            dir.normalize();

            var b_rad = a_b['vdw_radius']*rad_c;
            var e_rad = a_e['vdw_radius']*rad_c;

            start.x += b_rad*dir.x;
            start.y += b_rad*dir.y;
            start.z += b_rad*dir.z;

            end.x -= e_rad*dir.x;
            end.y -= e_rad*dir.y;
            end.z -= e_rad*dir.z;

            var middle = new THREE.Vector3(
                (start.x+end.x)*0.5,
                (start.y+end.y)*0.5,
                (start.z+end.z)*0.5);

            addStick(start, middle, a_b['color']);
            addStick(middle, end, a_e['color']);
        }
    }
    root.scale = cur_scale;
}

/*  position: THREE.Vector3,
    length: number,
    width: number,
    height: number */
function addBoundingBox(position, length, width, height) {
    var boxGeometry = new THREE.BoxGeometry(length, width, height);
    boxGeometry.dynamic = true;
    var color = new THREE.Color(0xff0000);

    var material = new THREE.MeshPhongMaterial( { color: color, transparent: true, opacity: 0.5 } );
    var boxMesh = new THREE.Mesh(boxGeometry, material);

    boxMesh.position.copy(position);
	boxMesh.scale.multiplyScalar( 20 );

    root.add(boxMesh);
}

function onWindowResize() {
    windowHalfX = window.innerWidth / 2;
    windowHalfY = window.innerHeight / 2;

	camera.aspect = window.innerWidth / window.innerHeight;
	camera.updateProjectionMatrix();

	renderer.setSize( window.innerWidth, window.innerHeight );

	render();
}

function onMouseMove(evt) {
    if (!mouseDown) {
        return;
    }

    evt.preventDefault();

    deltaX = evt.clientX - mouseX;
    deltaY = evt.clientY - mouseY;
    mouseX = evt.clientX;
    mouseY = evt.clientY;

    var rot_dy = -deltaX / 100;
    var rot_dx = deltaY / 100;

    // console.log('mouse move');
    // console.log(rot_dx);
    // console.log(rot_dy);
    // console.log('------------');

    root.rotation.y += rot_dy;
    root.rotation.x += rot_dx;

    var root_c = getBoundingBoxCenter();

    root.position.x -= root_c.x;
    root.position.y -= root_c.y;
    root.position.z -= root_c.z;
}


function onMouseDown(evt) {
    evt.preventDefault();

    mouseDown = true;
    
    mouseX = evt.clientX;
    mouseY = evt.clientY;
}



 function onMouseUp(evt) {
    evt.preventDefault();

    mouseDown = false;
}

function redrawAtomRadii() {
    var cur_rad;

    for(var child_key in root.children) {
        var child = root.children[child_key];
        if(child.name == 'atom') {
            if(vdw_rad_enabled) {
                cur_rad = root.children[child_key].geometry.parameters.radius;
                root.children[child_key].geometry.parameters.radius /= rad_c0;
                root.children[child_key].scale.x /= rad_c0;
                root.children[child_key].scale.y /= rad_c0;
                root.children[child_key].scale.z /= rad_c0;
            }
            else {
                cur_rad = root.children[child_key].geometry.parameters.radius;
                root.children[child_key].geometry.parameters.radius *= rad_c0;
                root.children[child_key].scale.x *= rad_c0;
                root.children[child_key].scale.y *= rad_c0;
                root.children[child_key].scale.z *= rad_c0;
            }
        }
    }
}

function onKeyDown(evt) {
    //alert('key pressed!');
    evt.preventDefault();
    var code = evt.keyCode;
    if (code != 86)
        return;

    // console.log('V pressed!');
    vdw_rad_enabled = !vdw_rad_enabled;

    redrawAtomRadii();
}

function onMouseWheel(e) {
    // console.log('scroling');
    var delta = e.deltaY || e.detail || e.wheelDelta;
    // console.log(`delta ${delta}`);
    var scale = cur_scale + delta/100;
    // console.log(`cur_scale ${scale}`)

    if(scale > MAX_SCALE || scale < MIN_SCALE)
        return;
    
    cur_scale = scale;
    root.scale.x = scale;
    root.scale.y = scale;
    root.scale.z = scale;
}

function createMenu() {

	for ( var m_id in MOLECULES ) {

		var button = document.createElement( 'button' );
        button.innerHTML = MOLECULES[m_id];
		menu.appendChild( button );

		var url = "molecules/" +  MOLECULES[ m_id ];

		button.addEventListener('click', generateButtonCallback( m_id ), false);

	}

}

function generateButtonCallback( mol_id ) {
	return function (event) {
		loadMol(mol_id);
	}
}

function loadMol(mol_id) {
    while ( root.children.length > 0 ) {
		var object = root.children[0];
		object.parent.remove(object);
	}
    vdw_rad_enabled = false;

    $.ajax({
        url: `/get/mol/${mol_id}/`,
        type: 'GET',
        dataType: 'json',
        success: function (data, textStatus) {
            if(data['result'] == 'error') {
                console.log(data['message'])
            }
            else if(data['result'] == 'success'){
				console.log(data['data']);
                var mol = JSON.parse(data['data']);
                console.log(mol);
                addMol(mol);
                render();
            }
        }
    });
}

function visualize_mols(lattice_conf) {
    init();
    vdw_rad_enabled = false;
    var mols = lattice_conf['mols'];
    
    for(var mol of mols) {
        addMol(mol);
    }

    var bb_pos = new THREE.Vector3(lattice_conf["bounding_box"]['position'][0], lattice_conf["bounding_box"]['position'][1], lattice_conf["bounding_box"]['position'][2]);
    console.log(lattice_conf['bounding_box']['position']);
    console.log(bb_pos);

    var bb_length = lattice_conf['bounding_box']['length'];
    var bb_width = lattice_conf['bounding_box']['width'];
    var bb_height = lattice_conf['bounding_box']['height'];
    console.log(bb_length);
    console.log(bb_width);
    console.log(bb_height);

    bb_pos.x += bb_length/2.0;
    bb_pos.y += bb_width/2.0;
    bb_pos.z += bb_height/2.0;

    bb_pos.multiplyScalar(20);

    addBoundingBox(bb_pos, bb_length, bb_width, bb_height);

    render();

}

function calcDistBtnFingers(touches) {
    var dx = touches[1].pageX - touches[0].pageX;
    var dy = touches[1].pageY - touches[0].pageY;

    return Math.sqrt(dx*dx + dy*dy);
}

// touch events
function onTouchMove(evt) {
    if ( evt.touches.length === 1 ) {
        // console.log('touch moved');

        evt.preventDefault();

        deltaTouchX = evt.touches[ 0 ].pageX - touchX;
        deltaTouchY = evt.touches[ 0 ].pageY - touchY;
        touchX = evt.touches[ 0 ].pageX;
        touchY = evt.touches[ 0 ].pageY;


        var rot_dy = -deltaTouchX / 100;
        var rot_dx = deltaTouchY / 100;

        // console.log(rot_dy);
        // console.log(rot_dx);
        // console.log('----------');

        root.rotation.y += rot_dy;
        root.rotation.x += rot_dx;

        var root_c = getBoundingBoxCenter();

        root.position.x -= root_c.x;
        root.position.y -= root_c.y;
        root.position.z -= root_c.z;
    }
    if ( evt.touches.length === 2 ) {
        evt.preventDefault();

        var dist = calcDistBtnFingers(evt.touches);

        deltaTouchDist = dist - touchDist;
        touchDist = dist;
        
        var scale = cur_scale + deltaTouchDist / 100;

        if (scale < MIN_SCALE || scale > MAX_SCALE)
            return;

        cur_scale = scale;
        root.scale.x = scale;
        root.scale.y = scale;
        root.scale.z = scale;
    }
}

function onTouchStart(evt) {
    if ( evt.touches.length === 1 ) {
        evt.preventDefault();

        touchX = evt.touches[ 0 ].pageX;
        touchY = evt.touches[ 0 ].pageY;
    }
    else if (evt.touches.length === 2) {
        evt.preventDefault();
        touchDist = calcDistBtnFingers(evt.touches);
    }
    else if (evt.touches.length === 3) {
        evt.preventDefault();
        vdw_rad_enabled = !vdw_rad_enabled;
        redrawAtomRadii();
    }
}
