function createDropdown(btn_title, items) {
    var btn_html = 
        `<button class="btn btn-primary dropdown-toggle" type="button" data-toggle="dropdown">
        	${btn_title}
			<span class="caret"></span>
		</button>`;
    var links = [];
    for(var item of items) {
        links.push(`<li><a href='${item['href']}'>${item['text']}</a></li>`);
    }
    var dd_lst = `<ul class="dropdown-menu">${links.join('')}</ul>`;
    var html = `<div class='btn-group'>${btn_html} ${dd_lst}</div>`;

    return html;
}

$('#btn_pack').click(function(evt){
    evt.preventDefault();
    var l = $('#txt_length').val();
    var w = $('#txt_width').val();
    var h = $('#txt_height').val();
    var mol_id = $('#sel_mol').prop("selectedIndex") + 1;
    var preloader_html = `
        <div id="circularG">
            <div id="circularG_1" class="circularG"></div>
            <div id="circularG_2" class="circularG"></div>
            <div id="circularG_3" class="circularG"></div>
            <div id="circularG_4" class="circularG"></div>
            <div id="circularG_5" class="circularG"></div>
            <div id="circularG_6" class="circularG"></div>
            <div id="circularG_7" class="circularG"></div>
            <div id="circularG_8" class="circularG"></div>
        </div>`;

    $('#controls').empty();
    $('#preloader').empty();
    $('#preloader').append(preloader_html);

    $.ajax({
        url: `http://127.0.0.1:3000/get/lattice/${mol_id}/?length=${l}&height=${h}&width=${w}`,
        type: 'GET',
        dataType: 'json',
        success: function (data, textStatus) {
            if(data['result'] == 'runtime error') {
                console.log(`debug_info:\n${data['debug']}`);
                console.error(`runtime error: ${data['errors']}`);
                return;
            }
            else if(data['result'] == 'execution error') {
                console.error(`execution error: ${data['error']}`);
                return;
            }

            if(typeof(Storage) !== 'undefined') {
                var storage;

                if(typeof(sessionStorage) !== 'undefined')
                    storage = sessionStorage;
                else if(typeof(localStorage) !== 'undefined')
                    storage = localStorage;
                else
                    alert('unsupported type of browser storage.\nTry to use another browser (like Google Chrome, Firefox, Opera and etc.)');

                if(typeof(storage.getItem('lattice_data')) !== 'undefined') {
                    storage.removeItem('lattice_data');
                }

                storage.setItem('lattice_data', data['data']);
                
                $('#preloader').empty();
                var download_href = `http://127.0.0.1:3000/download/lattice/${mol_id}/?length=${l}&width=${w}&height=${h}`;
                var dropdownItems = [
                    {'href': `${download_href}&format=json`, 'text': 'JSON'},
                    {'href': `${download_href}&format=mol2`, 'text': 'MOL2'},
                    {'href': `${download_href}&format=pdb`, 'text': 'PDB'},
                    {'href': `${download_href}&format=sdf`, 'text': 'SDF'}
                ];
                var ddHtml = createDropdown('Download', dropdownItems);

                $('#controls').append(ddHtml);
                $('#controls').append(`<a href='visualizer.html' target='_blank'>Preview</a><br/>`);

                $('.dropdown-toggle').dropdown();
            }

            else {
                alert('This browser don\'t support local storage.\nPlease try another one (like Google Chrome, Firefox, Opera and etc.)');
            }

        }
    });
});

