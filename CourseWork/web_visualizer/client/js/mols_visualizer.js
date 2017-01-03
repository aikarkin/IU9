var scene, camera, light, root, renderer;

var windowHalfX, windowHalfY;
var mouseX, mouseY;
var deltaX, deltaY;
var mouseDown = false;
var vdw_rad_enabled = false;
var rad_c0;
var MAX_SCALE = 15;
var MIN_SCALE = 1;
var cur_scale = 8;

var menu = document.getElementById( "menu" );
var cur_id = 1;

var MOLECULES = {
    1: "Water",
    2: "Ethylene chloride",
    3: "Caffeine",
    4: "Chloroform"
}

function addStick(start, end, color) {
    var boxGeometry = new THREE.BoxBufferGeometry( 1, 1, 1 );
    var s = new THREE.Vector3(start.x, start.y, start.z);
    var e = new THREE.Vector3(end.x, end.y, end.z);

    console.log(s);
    console.log(e);

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

    window.addEventListener( 'resize', onWindowResize, false );
}

function visualize_mol(mol, rad_c=0.3) {
    var atoms = mol["atoms"];
    rad_c0 = rad_c;

    for(var a_key in atoms) {
        var a = atoms[a_key];

        var sphereGeometry = new THREE.IcosahedronBufferGeometry( a['vdw_radius']*rad_c, 2 );
        var color = new THREE.Color();

        color.r = a['color'][0];
        color.g = a['color'][1];
        color.b = a['color'][2];

        var position = new THREE.Vector3();
        position.x = a['position'][0];
        position.y = a['position'][1];
        position.z = a['position'][2];

        sphereGeometry.dynamic = true;

        var material = new THREE.MeshPhongMaterial( { color: color } );
        var atom_obj = new THREE.Mesh( sphereGeometry, material );

        atom_obj.name = 'atom';
        atom_obj.position.copy(position);
        atom_obj.position.multiplyScalar( 20 );
		atom_obj.scale.multiplyScalar( 20);

        root.add(atom_obj);
    }

    if(rad_c != 1.0) {
        var bonds = mol["bonds"];

        for(var b of bonds) {
            //console.log(b);
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

    root.rotation.y -= deltaX / 100;
    root.rotation.x += deltaY / 100;
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

function onKeyDown(evt) {
    //alert('key pressed!');
    evt.preventDefault();
    var code = evt.keyCode;
    if (code != 86)
        return;

    console.log('V pressed!');
    vdw_rad_enabled = !vdw_rad_enabled;


    for(var child_key in root.children) {
        var child = root.children[child_key];
        if(child.name == 'atom') {
            var cur_rad;
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
            console.log('atom radius');
            console.log(root.children[child_key].geometry.parameters.radius);
            console.log(root.children[child_key]);
        }
    }
    //render();
    //root.updateMatrix();
}

function onMouseWheel(e) {
    console.log('scroling');
    var delta = e.deltaY || e.detail || e.wheelDelta;
    console.log(`delta ${delta}`);
    var scale = cur_scale + delta/100;
    console.log(`cur_scale ${scale}`)

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
        url: `http://127.0.0.1:3000/get/mol/${mol_id}/`,
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
                visualize_mol(mol);
                render();
            }
        }
    });
}

function visualize_mols(lattice_conf) {
    // while ( root.children.length > 0 ) {
	// 	var object = root.children[0];
	// 	object.parent.remove(object);
	// }
    init();
    vdw_rad_enabled = false;
    //console.log(lattice_conf);
    var mols = lattice_conf['mols'];
    for(var mol of mols) {
        //var mol = mols[m_key];
        //console.log(mol);
        visualize_mol(mol);
    }

    root.position.x += 200;
    root.position.y += 200;

    //console.log(mols);
}