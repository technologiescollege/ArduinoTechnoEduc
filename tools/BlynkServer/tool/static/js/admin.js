var app = angular.module('app', ['ng-admin']);
app.config(['NgAdminConfigurationProvider', function (nga) {
    // create an admin application
    var admin = nga.application('Blynk Administration', false)
        .baseApiUrl(location.protocol + '//' + window.location.hostname + (location.port == 80 ? '' : (':' + location.port)) + location.pathname + '/'); // main API endpoint
    // create a user entity
    // the API endpoint for this entity will be 'http://jsonplaceholder.typicode.com/users/:id
    var users = nga.entity('users').identifier(nga.field('id'));
    // set the fields of the user entity list view
    users.listView()
        .sortField('lastModifiedTs')
        .fields([
            nga.field('email').isDetailLink(true),
            nga.field('appName'),
            nga.field('# of projects').map(function (value, entry) {
                if (entry["profile.dashBoards"]) {
                    return entry["profile.dashBoards"].length;
                } else {
                    return 0;
                }
            }),
            nga.field('lastModifiedTs', 'datetime')
        ])
        .filters([
            nga.field('name').label('').pinned(true)
                .template('<div class="input-group"><input type="text" ng-model="value" placeholder="Search" class="form-control"></input><span class="input-group-addon"><i class="glyphicon glyphicon-search"></i></span></div>')
        ]);

    users.editionView()
        .title('Edit user "{{entry.values.email}}"')
        .fields(
            nga.field('email'),
            nga.field('name'),
            nga.field('pass', 'password'),
            nga.field('lastModifiedTs'),
            nga.field('energy'),
            nga.field('appName').editable(false),
            nga.field('region').editable(false),
            nga.field('lastLoggedIP').editable(false),
            nga.field('profile.dashBoards', 'embedded_list')
                .targetFields([
                    nga.field('id'),
                    nga.field('name'),
                    nga.field('createdAt'),
                    nga.field('theme', 'choice')
                        .choices([
                            {value: 'Blynk', label: 'Blynk'},
                            {value: 'SparkFun', label: 'SparkFun'}
                        ]),
                    nga.field('metadata', 'json'),
                    nga.field('widgets', 'embedded_list')
                        .targetFields([
                            nga.field('id', 'number'),
                            nga.field('label', 'string'),
                            nga.field('x').editable(false),
                            nga.field('y').editable(false),
                            nga.field('type').editable(false),
                            nga.field('width').editable(false),
                            nga.field('height').editable(false),
                            nga.field('value')
                        ]),
                    nga.field('devices', 'embedded_list')
                        .targetFields([
                            nga.field('id', 'number'),
                            nga.field('name', 'string'),
                            nga.field('boardType', 'choice')
                              .choices([
                                {value: 'Arduino 101', label: 'Arduino 101'},
                                {value: 'Arduino Due', label: 'Arduino Due'},
                                {value: 'Arduino Leonardo', label: 'Arduino Leonardo'},
                                {value: 'Arduino Mega', label: 'Arduino Mega'},
                                {value: 'Arduino Micro', label: 'Arduino Micro'},
                                {value: 'Arduino Mini', label: 'Arduino Mini'},
                                {value: 'Arduino MKR1000', label: 'Arduino MKR1000'},
                                {value: 'Arduino Nano', label: 'Arduino Nano'},
                                {value: 'Arduino Pro Micro', label: 'Arduino Pro Micro'},
                                {value: 'Arduino Pro Mini', label: 'Arduino Pro Mini'},
                                {value: 'Arduino UNO', label: 'Arduino UNO'},
                                {value: 'Arduino Yun', label: 'Arduino Yun'},
                                {value: 'Arduino Zero', label: 'Arduino Zero'},
                                {value: 'ESP8266', label: 'ESP8266'},
                                {value: 'Generic Board', label: 'Generic Board'},
                                {value: 'Intel Edison', label: 'Intel Edison'},
                                {value: 'Intel Galileo', label: 'Intel Galileo'},
                                {value: 'LinkIt ONE', label: 'LinkIt ONE'},
                                {value: 'Microduino Core+', label: 'Microduino Core+'},
                                {value: 'Microduino Core', label: 'Microduino Core'},
                                {value: 'Microduino CoreRF', label: 'Microduino CoreRF'},
                                {value: 'Microduino CoreUSB', label: 'Microduino CoreUSB'},
                                {value: 'NodeMCU', label: 'NodeMCU'},
                                {value: 'Particle Core', label: 'Particle Core'},
                                {value: 'Particle Electron', label: 'Particle Electron'},
                                {value: 'Particle Photon', label: 'Particle Photon'},
                                {value: 'Raspberry Pi 3 B', label: 'Raspberry Pi 3 B'},
                                {value: 'Raspberry Pi 2/A+/B+', label: 'Raspberry Pi 2/A+/B+'},
                                {value: 'Raspberry Pi B (Rev1)', label: 'Raspberry Pi B (Rev1)'},
                                {value: 'Raspberry Pi A/B (Rev2)', label: 'Raspberry Pi A/B (Rev2)'},
                                {value: 'RedBearLab CC3200/Mini', label: 'RedBearLab CC3200/Mini'},
                                {value: 'Seeed Wio Link', label: 'Seeed Wio Link'},
                                {value: 'SparkFun Blynk Board', label: 'SparkFun Blynk Board'},
                                {value: 'SparkFun ESP8266 Thing', label: 'SparkFun ESP8266 Thing'},
                                {value: 'SparkFun Photon RedBoard', label: 'SparkFun Photon RedBoard'},
                                {value: 'TI CC3200-LaunchXL', label: 'TI CC3200-LaunchXL'},
                                {value: 'TI Tiva C Connected', label: 'TI Tiva C Connected'},
                                {value: 'TinyDuino', label: 'TinyDuino'},
                                {value: 'WeMos D1', label: 'WeMos D1'},
                                {value: 'WeMos D1 mini', label: 'WeMos D1 mini'},
                                {value: 'Wildfire v2', label: 'Wildfire v2'},
                                {value: 'Wildfire v3', label: 'Wildfire v3'},
                                {value: 'Wildfire v4', label: 'Wildfire v4'},
                                {value: 'WiPy', label: 'WiPy'}
                            ]),
                            nga.field('token').editable(false),
                            nga.field('lastLoggedIP').editable(false),
                            nga.field('connectionType', 'choice')
                                .choices([
                                    {value: 'ETHERNET', label: 'ETHERNET'},
                                    {value: 'WI_FI', label: 'WI_FI'},
                                    {value: 'USB', label: 'USB'},
                                    {value: 'BLUETOOTH', label: 'BLUETOOTH'},
                                    {value: 'BLE', label: 'BLE'},
                                    {value: 'GSM', label: 'GSM'}
                                    ]
                            )
                        ]),
                    nga.field('tags', 'embedded_list')
                        .targetFields([
                            nga.field('id', 'number'),
                            nga.field('name', 'string')
                        ]),
                    nga.field('keepScreenOn', 'boolean'),
                    nga.field('isShared', 'boolean'),
                    nga.field('isActive', 'boolean'),
                    nga.field('isAppConnectedOn', 'boolean')
                ]),
            nga.field('dashShareTokens', 'json')
        );


    var libraryVersion = nga.entity('libraryVersion').identifier(nga.field('name')).url('hardwareInfo/version').readOnly();
    libraryVersion.listView()
        .title('Library versions')
        .perPage(50)
        .batchActions([])
        .sortField('count')
        .fields([
            nga.field('name').label('Blynk library version'),
            nga.field('count').label('Count')
        ]);

    var cpuType = nga.entity('cpuType').identifier(nga.field('name')).url('hardwareInfo/cpuType').readOnly();
    cpuType.listView()
        .title('CPU types')
        .perPage(50)
        .batchActions([])
        .sortField('count')
        .fields([
            nga.field('name').label('CPU type'),
            nga.field('count').label('Count')
        ]);

    var connectionType = nga.entity('connectionType').identifier(nga.field('name')).url('hardwareInfo/connectionType').readOnly();
    connectionType.listView()
        .title('Connection types')
        .perPage(50)
        .batchActions([])
        .sortField('count')
        .fields([
            nga.field('name').label('Connection type'),
            nga.field('count').label('Count')
        ]);

    var hardwareBoards = nga.entity('hardwareBoards').identifier(nga.field('name')).url('hardwareInfo/boards').readOnly();
    hardwareBoards.listView()
        .title('Board types')
        .perPage(50)
        .batchActions([])
        .sortField('count')
        .fields([
            nga.field('name').label('Hardware board type'),
            nga.field('count').label('Count')
        ]);


    var realtime = nga.entity('realtime').url('stats/realtime').readOnly();
    realtime.listView()
        .title('Realtime stats')
        .batchActions([])
        .sortField('count')
        .fields([
            nga.field('oneMinRate').label('1 min request rate'),
            nga.field('total').label('Total registrations'),
            nga.field('active').label('Logged in 24h'),
            nga.field('active3').label('Logged in 72h'),
            nga.field('connected').label('Hard and App connected'),
            nga.field('onlineApps').label('App connections'),
            nga.field('onlineHards').label('Hardware connections')
            ]);

    var requestsPerUser = nga.entity('requestsPerUser').identifier(nga.field('name')).url('stats/requestsPerUser').readOnly();
    requestsPerUser.listView()
        .title('Requests per user')
        .perPage(50)
        .batchActions([])
        .sortField('hardRate')
        .fields([
            nga.field('name').label('User'),
            nga.field('hardRate').label('Hardware requests per second'),
            nga.field('appRate').label('Application requests per second')
        ]);

    var messages = nga.entity('messages').identifier(nga.field('name')).url('stats/messages').readOnly();
    messages.listView()
        .title('Messages')
        .perPage(50)
        .batchActions([])
        .sortField('count')
        .fields([
            nga.field('name').label('Message'),
            nga.field('count').label('Count')
        ]);

    var boards = nga.entity('boards').identifier(nga.field('name')).url('stats/boards').readOnly();
    boards.listView()
        .title('Board Types')
        .perPage(50)
        .batchActions([])
        .sortField('count')
        .fields([
            nga.field('name').label('Board Name'),
            nga.field('count').label('Count')
        ]);

    var facebookLogins = nga.entity('facebookLogins').identifier(nga.field('name')).url('stats/facebookLogins').readOnly();
    facebookLogins.listView()
        .title('Login Types')
        .perPage(50)
        .batchActions([])
        .sortField('count')
        .fields([
            nga.field('name').label('Login Type'),
            nga.field('count').label('Count')
        ]);


    var widgets = nga.entity('widgets').identifier(nga.field('name')).url('stats/widgets').readOnly();
    widgets.listView()
        .title('Widgets')
        .perPage(50)
        .batchActions([])
        .sortField('count')
        .fields([
            nga.field('name').label('Widget'),
            nga.field('count').label('Count')
        ]);

    var projectsPerUser = nga.entity('projectPerUser').identifier(nga.field('name')).url('stats/projectsPerUser').readOnly();
    projectsPerUser.listView()
        .title('Project per user')
        .perPage(50)
        .batchActions([])
        .sortField('count')
        .fields([
            nga.field('name').label('# of project per user'),
            nga.field('count').label('Count')
        ]);


    var filledSpace = nga.entity('filledSpace').identifier(nga.field('name')).url('stats/filledSpace').readOnly();
    filledSpace.listView()
        .title('Filled space')
        .perPage(100)
        .batchActions([])
        .sortField('count')
        .fields([
            nga.field('name').label('# of cells per project'),
            nga.field('count').label('Count')
        ]);

    var userProfileSize = nga.entity('userProfileSize').identifier(nga.field('name')).url('stats/userProfileSize').readOnly();
    userProfileSize.listView()
        .title('Size of user profile')
        .perPage(50)
        .batchActions([])
        .sortField('count')
        .fields([
            nga.field('name').label('User'),
            nga.field('count').label('Size in bytes')
        ]);

    var webHookHosts = nga.entity('webHookHosts').identifier(nga.field('name')).url('stats/webHookHosts').readOnly();
    webHookHosts.listView()
        .title('Webhook hosts')
        .perPage(50)
        .batchActions([])
        .sortField('count')
        .fields([
            nga.field('name').label('Host'),
            nga.field('count').label('Number')
        ]);

    var ips = nga.entity('ips').identifier(nga.field('name')).url('stats/ips').readOnly();
    ips.listView()
        .title('IP List')
        .perPage(50)
        .batchActions([])
        .sortField('ip')
        .fields([
            nga.field('ip'),
            nga.field('name')
        ])
        .filters([
            nga.field('ip').label('').pinned(true)
                .template('<div class="input-group"><input type="text" ng-model="value" placeholder="Search" class="form-control"></input><span class="input-group-addon"><i class="glyphicon glyphicon-search"></i></span></div>')
        ]);

    var config = nga.entity('config').identifier(nga.field('name'));
    config.listView()
        .title('Configurations')
        .sortField('name')
        .batchActions([])
        .fields([
            nga.field('name').isDetailLink(true)
        ]);


    config.editionView()
        .title('Edit configuration "{{entry.values.name}}"')
        .fields(
            nga.field('name').editable(false),
            nga.field('body', 'text')
        );

        // customize menu
    admin.menu(nga.menu().autoClose(false)
            .addChild(nga.menu(users).icon('<span class="glyphicon glyphicon-user"></span>'))
            .addChild(nga.menu().title('Stats')
                .addChild(nga.menu(realtime).title('Realtime').icon(''))
                .addChild(nga.menu(requestsPerUser).title('Request per user').icon(''))
                .addChild(nga.menu(messages).title('Messages').icon(''))
                .addChild(nga.menu(boards).title('Board types').icon(''))
                .addChild(nga.menu(facebookLogins).title('Login types').icon(''))
                .addChild(nga.menu(widgets).title('Widgets').icon(''))
                .addChild(nga.menu(projectsPerUser).title('Projects per user').icon(''))
                .addChild(nga.menu(filledSpace).title('Cells per project').icon(''))
                .addChild(nga.menu(userProfileSize).title('Size of user profile').icon(''))
                .addChild(nga.menu(webHookHosts).title('Webhook hosts').icon(''))
                .addChild(nga.menu(ips).title('IPs').icon(''))
            )
            .addChild(nga.menu().title('Hardware Info')
                .addChild(nga.menu(libraryVersion).title('Library versions').icon(''))
                .addChild(nga.menu(cpuType).title('CPU types').icon(''))
                .addChild(nga.menu(hardwareBoards).title('Hardware boards').icon(''))
                .addChild(nga.menu(connectionType).title('Connection types').icon(''))
            )
            .addChild(nga.menu(config))
    );


    // add the user entity to the admin application
    admin.addEntity(users);
    admin.addEntity(requestsPerUser);
    admin.addEntity(realtime);
    admin.addEntity(messages);
    admin.addEntity(boards);
    admin.addEntity(facebookLogins);
    admin.addEntity(widgets);
    admin.addEntity(projectsPerUser);
    admin.addEntity(filledSpace);
    admin.addEntity(userProfileSize);
    admin.addEntity(libraryVersion);
    admin.addEntity(cpuType);
    admin.addEntity(hardwareBoards);
    admin.addEntity(connectionType);
    admin.addEntity(webHookHosts);
    admin.addEntity(ips);
    admin.addEntity(config);

    admin.dashboard(nga.dashboard().addCollection(
            nga.collection(realtime)
                .title('Realtime stats')
                .batchActions([])
                .sortField('count')
                .fields([
                    nga.field('oneMinRate').label('1 min request rate'),
                    nga.field('total').label('Total registrations'),
                    nga.field('active').label('Logged in 24h'),
                    nga.field('active3').label('Logged in 72h'),
                    nga.field('connected').label('Hard and App connected'),
                    nga.field('onlineApps').label('App connections'),
                    nga.field('onlineHards').label('Hardware connections')
                ])

    )
    );

    // attach the admin application to the DOM and execute it
    nga.configure(admin);
}]);