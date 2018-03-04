/* global Blockly, hopscotch, tutorials, JsDiff, DwenguinoBlocklyLanguageSettings, MSG, BlocklyStorage */

if (!window.dwenguinoBlocklyServer) {
  dwenguinoBlocklyServer = false;
}

var DwenguinoBlockly = {
    simButtonStateClicked: false,

    workspace: null,
    recording: "",
    sessionId: null,
    tutorialId: null,

    serverUrl: 'http://localhost:8081',

    //General settings for this session, these are used for data logging during experiments
    agegroupSetting: "",
    genderSetting: "",  //TODO: add this to the modal dialog
    activityIdSetting: "",
    tutorialIdSetting: "",
    computerId: "-1",
    workshopId: "-1",
    difficultyLevel: 1,
    simulatorState: "off",

    initDwenguinoBlockly: function(){
        //set keypress event listerner to show test environment window
        var keys = {};
        $(document).keydown(function (e) {
            keys[e.which] = true;
            if (keys[69] && keys[83] && keys[84]){
                console.log("starting test environment");
                $('#myModal').modal('show');
                var db_now = new Date();

                var db_day = ("0" + db_now.getDate()).slice(-2);
                var db_month = ("0" + (db_now.getMonth() + 1)).slice(-2);

                var db_today = db_now.getFullYear()+"-"+(db_month)+"-"+(db_day) ;
                $('#activity_date').val(db_today);
            }
        });

        $(document).keyup(function (e) {
            delete keys[e.which];
        });

        //code to init the bootstrap modal dialog
        $("#submit_modal_dialog_button").click(function(){
            DwenguinoBlockly.agegroupSetting = $("input[name=optradio]:checked").val();
            DwenguinoBlockly.activityIdSetting = $("#activity_identifier").val();
            var activity_date = $("#activity_date").val();
            console.log("[act;" + (DwenguinoBlockly.agegroupSetting || "")
                + ";" + (DwenguinoBlockly.activityIdSetting || "")
                + ";" + (activity_date || "") + "]");
        });

        DwenguinoBlockly.sessionId = window.sessionStorage.loadOnceSessionId;
        delete window.sessionStorage.loadOnceSessionId;
        if (!DwenguinoBlockly.sessionId /*&& dwenguinoBlocklyServer*/){
            // Try to get a new sessionId from the server to keep track
            $.ajax({
                type: "GET",
                url: this.serverUrl + "/sessions/newId"}
            ).done(function(data){
                console.debug('sessionId is set to', data);
                DwenguinoBlockly.sessionId = data;
            }).fail(function(response, status)  {
                console.warn('Failed to fetch sessionId:', status);
            });
        }

        //Restore recording after language change
        DwenguinoBlockly.recording = window.sessionStorage.loadOnceRecording || "";
        delete window.sessionStorage.loadOnceRecording;

        //init slider control
        $( "#db_menu_item_difficulty_slider" ).slider({
            value:0,
            min: 0,
            max: 2,
            step: 1,
            slide: function( event, ui ) {
                DwenguinoBlockly.setDifficultyLevel(ui.value);
                console.log(ui.value);
                DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("setDifficultyLevel", ui.value));
            }
        });
        $( "#db_menu_item_difficulty_slider_input" ).val( "$" + $( "#db_menu_item_difficulty_slider" ).slider( "value" ) );

        //init resizable panels
        $( "#db_blockly" ).resizable({
            handles: "e"
        });

        $( "#db_blockly" ).resize(function(){
          DwenguinoBlockly.onresize();
          Blockly.svgResize(DwenguinoBlockly.workspace);
        });

        //show/hide simulator
        $("#db_menu_item_simulator").click(function(){
            if (this.simButtonStateClicked){
                $("#db_blockly").width('100%');
                //$("#db_blockly").animate({width: "+=0.05%%"}, {duration: 1000});
                this.simButtonStateClicked = false;
                DwenguinoBlockly.simulatorState = "off";
                DwenguinoSimulation.handleSimulationStop();
            }else{
                $("#db_blockly").width('50%');
                //$("#db_blockly").animate({width: "-=0.05%"}, {duration: 1000});
                this.simButtonStateClicked = true;
                DwenguinoBlockly.simulatorState = "on";
                DwenguinoSimulation.setupEnvironment();
            }
            DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("simButtonStateClicked", DwenguinoBlockly.simulatorState));
            DwenguinoBlockly.onresize();
            Blockly.svgResize(DwenguinoBlockly.workspace);
        });

        //save/upload buttons
        $("#db_menu_item_run").click(DwenguinoBlockly.runEventHandler);

        $("#db_menu_item_upload").click(function(){
          var xml = "";
      		try {
      			xml = Blockly.Xml.textToDom(dwenguinoBlocklyServer ? dwenguinoBlocklyServer.loadBlocks() : localStorage.workspaceXml);
      			DwenguinoBlockly.workspace.clear();
      			console.log(xml);
      			Blockly.Xml.domToWorkspace(xml, DwenguinoBlockly.workspace);
      		    } catch (e) {}
      		var count = DwenguinoBlockly.workspace.getAllBlocks().length;
      		DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("uploadClicked", xml));
        });



        $("#db_menu_item_download").click(function(){
            var xml = Blockly.Xml.workspaceToDom(DwenguinoBlockly.workspace);
            var data = Blockly.Xml.domToText(xml);
            if (dwenguinoBlocklyServer){
                dwenguinoBlocklyServer.saveBlocks(data);
            } else {
                console.log(data);
                localStorage.workspaceXml = data;
            }
            DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("downloadClicked", ""));
        });
        //dropdown menu code
         $(".dropdown-toggle").dropdown();
         console.log(tutorials);

         $.each(tutorials, function(index, arrayElement){
           var newLi = $("<li>").attr("class", "dropdownmenuitem").attr("id", arrayElement.id).attr("role", "presentation").html(arrayElement.label);
           newLi.click(function(){
             DwenguinoBlockly.tutorialId = arrayElement.id;
             DwenguinoBlockly.tutorialIdSetting = DwenguinoBlockly.tutorialId;
             hopscotch.configure({showPrevButton: "true"}); //configure tutorial views
             hopscotch.startTour(arrayElement);
             DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("startTutorial", DwenguinoBlockly.tutorialIdSetting));
           });
           $("#dropdownMenuTuts").append(newLi);
         });

         //following event listener is only a test --> remove later!
         $("#db_menu_item_dwengo_robot_teacher_image").click(function(){
            //DwenguinoBlockly.takeSnapshotOfWorkspace();
            //DwenguinoBlockly.build("void setup() {} void loop() {}");
            //DwenguinoBlockly.build("void main() {}");
            //DwenguinoBlockly.listPorts();
         });

         $("#language1").click(function(){
            DwenguinoBlockly.language = "cpp";
            DwenguinoBlockly.renderCode();
         });

         $("#language2").click(function(){
            DwenguinoBlockly.language = "js";
            DwenguinoBlockly.renderCode();
         });

         $("#blocklyDiv").click(function(){
           DwenguinoSimulation.handleSimulationStop();
         });

         // Process blockly events and log them
         DwenguinoBlockly.workspace.addChangeListener(function(event){
           // Stop de simulator
           DwenguinoSimulation.handleSimulationStop();
           console.log("blockly event");
           if (event.type == "create"){
             var data = {
               xml: event.xml,
               ids: event.ids
             }
             DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("blocklyBlockCreate", data));
           } else if (event.type == "delete"){
             var data = {
               oldXml: event.oldXml,
               ids: event.ids
             }
             DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("blocklyBlockDelete", data));
           } else if (event.type == "move"){
             var data = {
               oldParentId: event.oldParentId,
               oldInputName: event.oldInputName,
               oldCoordinate: event.oldCoordinate,
               newParentId: event.newParentId,
               newInputName: event.newInputName,
               newCoordinate: event.newCoordinate
             }
             DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("blocklyBlockMove", data));
           } else if (event.type == "createVar"){
             var data = {
               varType: event.varType,
               varName: event.varName,
               varId: event.varId
             }
             DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("blocklyVarCreate", data));
           } else if (event.type == "deleteVar"){
             var data = {
               varType: event.varType,
               varName: event.varName,
               varId: event.varId
             }
             DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("blocklyVarDelete", data));
           } else if (event.type == Blockly.Events.VAR_RENAME){
             var data = {
               oldName: event.oldName,
               newName: event.newName,
               varId: event.varId
             }
             DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("blocklyVarRename", data));
           } else if (event.type == Blockly.Events.UI){
             var data = {
               element: event.element,
               oldValue: event.oldValue,
               newValue: event.newValue
             }
             DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("blocklyUI", data));
           } else if (event.type == Blockly.Events.CHANGE){
             var data = {
               element: event.element,
               name: event.name,
               oldValue: event.oldValue,
               newValue: event.newValue
             };
             DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("blocklyChange", data));
           }
         });

    },
    createEvent: function(eventName, data){
      var event = {
        "name": eventName,
        "timestamp": $.now(),
        "simulatorState": DwenguinoBlockly.simulatorState,
        "selectedDifficulty": DwenguinoBlockly.difficultyLevel,
        "activeTutorial": DwenguinoBlockly.tutorialIdSetting,
        "groupId": DwenguinoBlockly.activityIdSetting,
        "computerId": DwenguinoBlockly.computerId,
        "workshopId": DwenguinoBlockly.workshopId,
        "data": data
      };
      return event;
    },

    endTutorial: function(){
        DwenguinoBlockly.tutorialId = "";
        DwenguinoBlockly.tutorialIdSetting = "";
        DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("endTutorial", ""));
    },
    /*
    * This function submits an event to the python logging server.
    */
    recordEvent: function(eventToRecord){
      var serverSubmission = {
        "sessionId": DwenguinoBlockly.sessionId,
        "agegroup": DwenguinoBlockly.agegroupSetting,
        "gender": DwenguinoBlockly.genderSetting,
        "activityId": DwenguinoBlockly.activityId,
        "timestamp": $.now(),
        "event": eventToRecord
      };
      if (DwenguinoBlockly.sessionId !== undefined){
        $.ajax({
            type: "POST",
            url: this.serverUrl + "/sessions/update",
            data: {"logEntry": JSON.stringify(serverSubmission)},
        }).done(function(data){
            console.debug('Recording submitted', data);
        }).fail(function(response, status)  {
            console.warn('Failed to submit recording:', status);
        });
      }
    },

    runEventHandler: function(){
      $("#db_menu_item_run").off("click");
      $("#db_menu_item_dwengo_robot_teacher_image").attr("src", "img/gear_animation.gif");
      $("#db_menu_item_dwengo_robot_teacher_image").css({padding: "10px 25px", maxHeight: "100%", float: "right"});
      $("#db_menu_item_run").css({color: "gray"});
      setTimeout(function(){
        console.log("timeout")
        $("#db_menu_item_run").click(DwenguinoBlockly.runEventHandler);
        $("#db_menu_item_run").css({color: "black"});
        $("#db_menu_item_run").hover(function() {
          $(this).css({color: "#8bab42"});
        });
        $("#db_menu_item_run").mouseleave(function() {
          $(this).css({color: "black"});
        });
        $("#db_menu_item_dwengo_robot_teacher_image").css({padding: "10px", maxHeight: "100%", float: "right"});
        $("#db_menu_item_dwengo_robot_teacher_image").attr("src", "img/dwengo_robot_plain.svg");
      }, 5000);
      var code = Blockly.Arduino.workspaceToCode(DwenguinoBlockly.workspace);
      if ((typeof dwenguinoBlocklyServer) != 'undefined' && dwenguinoBlocklyServer){
          dwenguinoBlocklyServer.uploadCode(code);
      }
      console.log(code.replace(/\r?\n|\r/g, "\n"));
      //DwenguinoBlockly.build(code.replace(/\r?\n|\r/g, "\\n"));
      DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("runClicked", ""));
    },

    prevWorkspaceXml: "",
    /**
    *   Take a snapshot of the current timestamp, simulatorstate, selectedDifficulty, activeTutorial and blocks in the workspace.
    */
    takeSnapshotOfWorkspace: function(){
        console.log("taking snapshot");
        var xml = Blockly.Xml.workspaceToDom(DwenguinoBlockly.workspace);
        var text = Blockly.Xml.domToText(xml);
        if (text != DwenguinoBlockly.prevWorkspaceXml){
            DwenguinoBlockly.prevWorkspaceXml = text;
            DwenguinoBlockly.recordEvent(DwenguinoBlockly.createEvent("changedWorkspace", text));
        }
    },

    /**
    *   Log the code changes of the user
    *   @param {type} event
    */
    logCodeChange: function(event){
        DwenguinoBlockly.takeSnapshotOfWorkspace();
    },

    previouslyRenderedCode: null,
    language: "cpp",
    /**
     * Populate the currently selected pane with content generated from the blocks.
     */
    renderCode: function() {
        var arduino_content = document.getElementById("content_arduino");
        //var xml_content = document.getElementById("content_xml");

        // transform code
        if (DwenguinoBlockly.language === "cpp") {
            var code = Blockly.Arduino.workspaceToCode(DwenguinoBlockly.workspace);
        }
        else if (DwenguinoBlockly.language === "js") {
            var code = Blockly.JavaScript.workspaceToCode(DwenguinoBlockly.workspace);
        }

        // display code
        if (DwenguinoBlockly.previouslyRenderedCode === null){
            document.getElementById('content_arduino').innerHTML =
                prettyPrintOne(code.replace(/</g, "&lt;").replace(/>/g, "&gt;"), 'cpp', false);
                DwenguinoBlockly.previouslyRenderedCode = code;
        }
        else if (code !== DwenguinoBlockly.previouslyRenderedCode) {
            //When the redered code changed log the block structures
            //Do this because when the user moves blocks we do not want to log anything
            //We want to log code progression not code movement
            DwenguinoBlockly.logCodeChange();

            var diff = JsDiff.diffWords(DwenguinoBlockly.previouslyRenderedCode, code);
            var resultStringArray = [];
            for (var i = 0; i < diff.length; i++) {
              if (!diff[i].removed) {
                var escapedCode = diff[i].value.replace(/</g, "&lt;")
                                               .replace(/>/g, "&gt;");
                if (diff[i].added) {
                  resultStringArray.push(
                      '<span class="code_highlight_new">' + escapedCode + '</span>');
                } else {
                  resultStringArray.push(escapedCode);
                }
              }
            }
            document.getElementById('content_arduino').innerHTML =
                prettyPrintOne(resultStringArray.join(''), 'cpp', false);
                DwenguinoBlockly.previouslyRenderedCode = code;
         }

    },

    setDifficultyLevel: function(level){
        DwenguinoBlockly.difficultyLevel = level;
        $("#toolbox").load("levels/lvl" + level + ".xml", function(){
            DwenguinoBlockly.doTranslation();
            DwenguinoBlockly.workspace.updateToolbox(document.getElementById("toolbox"));
        });
    },

    /*
      The following functions load the contents of a block xml file into the workspace.
    */
    loadBlocksFile: function(filename){
      DwenguinoBlockly.loadBlocksFileContent(filename);
    },

    loadFileXmlIntoWorkspace: function(xml_content){
      console.log("loading into workspace");
      DwenguinoBlockly.setWorkspaceBlockFromXml(xml_content);
    },

    loadBlocksFileContent: function(filename){
      $.ajax({
        url: filename,
        success: function (data){
          var serializedToString = (new XMLSerializer()).serializeToString(data);
          DwenguinoBlockly.loadFileXmlIntoWorkspace(serializedToString);
        }
      });
    },


    onresize: function(){
        var blocklyArea = document.getElementById('db_blockly');
        var blocklyDiv = document.getElementById('blocklyDiv');
        // Compute the absolute coordinates and dimensions of blocklyArea.
        var element = blocklyArea;
        var x = 0;
        var y = 0;
        do {
            x += element.offsetLeft;
            y += element.offsetTop;
            element = element.offsetParent;
        } while (element);
        // Position blocklyDiv over blocklyArea.
        blocklyDiv.style.left = x + 'px';
        blocklyDiv.style.top = y + 'px';
        blocklyDiv.style.width = blocklyArea.offsetWidth + 'px';
        blocklyDiv.style.height = blocklyArea.offsetHeight + 'px';
    },

    injectBlockly: function(){
        var blocklyArea = document.getElementById('db_blockly');
        var blocklyDiv = document.getElementById('blocklyDiv');
        DwenguinoBlockly.workspace = Blockly.inject(blocklyDiv,
            {
                toolbox: document.getElementById('toolbox'),
                media: "./img/",
                zoom: {controls: true, wheel: true}
            });
        window.addEventListener('resize', DwenguinoBlockly.onresize, false);
        DwenguinoBlockly.onresize();
        Blockly.svgResize(DwenguinoBlockly.workspace);
        DwenguinoBlockly.workspace.addChangeListener(DwenguinoBlockly.renderCode);
    },

    changeLanguage: function() {
        // Store the blocks for the duration of the reload.
        // This should be skipped for the index page, which has no blocks and does
        // not load Blockly.
        // Also store the recoring up till now.
        // MSIE 11 does not support sessionStorage on file:// URLs.
        if (typeof Blockly !== 'undefined' && window.sessionStorage) {
            var xml = Blockly.Xml.workspaceToDom(DwenguinoBlockly.workspace);
            var text = Blockly.Xml.domToText(xml);
            window.sessionStorage.loadOnceBlocks = text;
            window.sessionStorage.loadOnceRecording = DwenguinoBlockly.recording;
            window.sessionStorage.loadOnceSessionId = DwenguinoBlockly.sessionId;
        }

        var languageMenu = document.getElementById('db_menu_item_language_selection');
        var newLang = encodeURIComponent(languageMenu.options[languageMenu.selectedIndex].value);
        var search = window.location.search;
        if (search.length <= 1) {
            search = '?lang=' + newLang;
        } else if (search.match(/[?&]lang=[^&]*/)) {
            search = search.replace(/([?&]lang=)[^&]*/, '$1' + newLang);
        } else {
            search = search.replace(/\?/, '?lang=' + newLang + '&');
        }

        window.location = window.location.protocol + '//' +
        window.location.host + window.location.pathname + search;
    },

    /**
     * User's language (e.g. "en").
     * @type {string}
     */
    LANG: DwenguinoBlocklyLanguageSettings.getLang(),

    isRtl: function(){
        return false;
    },

    /**
     * Initialize the page language.
     */
    initLanguage: function() {
      // Set the HTML's language and direction.
      var rtl = DwenguinoBlockly.isRtl();
      document.dir = rtl ? 'rtl' : 'ltr';
      document.head.parentElement.setAttribute('lang', DwenguinoBlockly.LANG);

      // Sort languages alphabetically.
      var languages = [];
      for (var lang in DwenguinoBlocklyLanguageSettings.LANGUAGE_NAME) {
        languages.push([DwenguinoBlocklyLanguageSettings.LANGUAGE_NAME[lang], lang]);
      }
      var comp = function(a, b) {
        // Sort based on first argument ('English', 'Русский', '简体字', etc).
        if (a[0] > b[0]) return 1;
        if (a[0] < b[0]) return -1;
        return 0;
      };
      languages.sort(comp);
      // Populate the language selection menu.
      var languageMenu = document.getElementById('db_menu_item_language_selection');
      languageMenu.options.length = 0;
      for (var i = 0; i < languages.length; i++) {
        var tuple = languages[i];
        var lang = tuple[tuple.length - 1];
        var option = new Option(tuple[0], lang);
        if (lang === DwenguinoBlockly.LANG) {
          option.selected = true;
        }
        languageMenu.options.add(option);
      }
      languageMenu.addEventListener('change', DwenguinoBlockly.changeLanguage, true);

  },

    doTranslation: function() {
        // Inject language strings.
        document.title += ' ' + MSG['title'];
        //document.getElementById('title').textContent = MSG['title'];
        //document.getElementById('tab_blocks').textContent = MSG['blocks'];

        //document.getElementById('linkButton').title = MSG['linkTooltip'];
        document.getElementById('db_menu_item_run').title = MSG['runTooltip'];
        document.getElementById('db_menu_item_upload').title = MSG['loadBlocksFileTooltip'];
        document.getElementById('db_menu_item_download').title = MSG['saveBlocksFileTooltip'];
        document.getElementById('db_menu_item_simulator').title = MSG['toggleSimulator'];
        //document.getElementById('t

        var tutorials = ['tutsIntroduction', 'tutsHelloDwenguino', 'tutsBlink', 'tutsHelloRobot',
        'tutsNameOnLcd', 'tutsBlinkLED', 'tutsLedOnButtonPress', 'tutsBitPatternOnLeds',
      'tutsAllButtons', 'tutsDriveForward', 'tutsRideInSquare', 'tutsRideToWall', 'tutsAvoidWall'];
        for (var i = 0; i < tutorials.length ; i++){
            var element = document.getElementById(tutorials[i]);
            if (element){
                element.innerHTML = MSG[tutorials[i]];
            }
        }

        var categories = ['catLogic', 'catLoops', 'catMath', 'catText', 'catLists',
            'catColour', 'catVariables', 'catFunctions', 'catBoardIO', 'catDwenguino', 'catArduino'];
        for (var i = 0, cat; cat = categories[i]; i++) {
            var element = document.getElementById(cat);
            if (element) {
                element.setAttribute('name', MSG[cat]);
            }

        }
        var textVars = document.getElementsByClassName('textVar');
        for (var i = 0, textVar; textVar = textVars[i]; i++) {
            textVar.textContent = MSG['textVariable'];
        }
        var listVars = document.getElementsByClassName('listVar');
        for (var i = 0, listVar; listVar = listVars[i]; i++) {
            listVar.textContent = MSG['listVariable'];
        }
    },

    /**
     * Load blocks saved on App Engine Storage or in session/local storage.
     * @param {string} defaultXml Text representation of default blocks.
     */
    loadBlocks: function(defaultXml) {
      try {
        var loadOnce = window.sessionStorage.loadOnceBlocks;
      } catch(e) {
        // Firefox sometimes throws a SecurityError when accessing sessionStorage.
        // Restarting Firefox fixes this, so it looks like a bug.
        var loadOnce = null;
      }
      if ('BlocklyStorage' in window && window.location.hash.length > 1) {
        // An href with #key trigers an AJAX call to retrieve saved blocks.
        BlocklyStorage.retrieveXml(window.location.hash.substring(1));
      } else if (loadOnce) {
        // Language switching stores the blocks during the reload.
        delete window.sessionStorage.loadOnceBlocks;
        var xml = Blockly.Xml.textToDom(loadOnce);
        Blockly.Xml.domToWorkspace(xml, DwenguinoBlockly.workspace);
      } else if (defaultXml) {
        // Load the editor with default starting blocks.
        var xml = Blockly.Xml.textToDom(defaultXml);
        Blockly.Xml.domToWorkspace(xml, DwenguinoBlockly.workspace);
        // Set empty recording
        DwenguinoBlockly.recording = "";
      } else if ('BlocklyStorage' in window) {
        // Restore saved blocks in a separate thread so that subsequent
        // initialization is not affected from a failed load.
        window.setTimeout(BlocklyStorage.restoreBlocks, 0);
      }
  },


  build: function(sketchCode){
    $("#db_menu_item_run").off("click");
    $("#db_menu_item_dwengo_robot_teacher_image").attr("src", "img/gear_animation.gif");
    console.log('{"Code": "' + sketchCode + '"}');
    $.ajax({
        type: 'POST',
        url: DwenguinoBlockly.serverUrl + "/build",
        contentType: "application/json",
        async: true,
        data: '{"Code": "' + sketchCode + '"}',
        error: function(msg){
          $("#db_menu_item_run").click(DwenguinoBlockly.runEventHandler);
          $("#db_menu_item_dwengo_robot_teacher_image").attr("src", "img/dwengo_robot_plain.svg");
          console.log(msg);
        },
        success: function(msg){
          console.log(msg);
          if (msg.base64HexCode != ""){
            console.log("compilation succesful, uploading");
            DwenguinoBlockly.upload(msg.base64HexCode);
          }
        }
    });
  },
  listPorts: function(compilationResult){
    $.ajax({
        type: 'GET',
        url: DwenguinoBlockly.serverUrl + "/listPorts",
        error: function(msg){
          console.log(msg);
        },
        success: function(msg){
          console.log(msg);
        }
    });
  },

  upload: function(base64HexCode){
    var requestData = '{"port": "' + '/dev/ttyACM0' + '", "base64HexCode": "' + base64HexCode + '"}';
    if (window["WebSocket"]) {
       conn = new WebSocket("wss://localhost:8991/ws");
       conn.onclose = function(evt) {
           appendLog($("<div><b>Connection closed.</b></div>"))
       }
       conn.onmessage = function(evt) {
           appendLog($("<div/>").text(evt.data))
       }

       if (!conn) {
            return false;
        }
        conn.send("open /dev/ttyACM0 57600");
        msg.val("");
   } else {
       appendLog($("<div><b>Your browser does not support WebSockets.</b></div>"))
   }
    console.log(requestData);
    $.ajax({
        type: 'POST',
        url: DwenguinoBlockly.serverUrl + "/uploadToDwenguino",
        contentType: "application/json",
        async: true,
        data: requestData,
        error: function(msg){
          $("#db_menu_item_run").click(DwenguinoBlockly.runEventHandler);
          $("#db_menu_item_dwengo_robot_teacher_image").attr("src", "img/dwengo_robot_plain.svg");
          console.log(msg);
        },
        success: function(msg){
          $("#db_menu_item_run").click(DwenguinoBlockly.runEventHandler);
          $("#db_menu_item_dwengo_robot_teacher_image").attr("src", "img/dwengo_robot_plain.svg");
          console.log(msg);
        }
    });
  },

  //TODO: remove following function: not used anywhere
    setWorkspaceBlockFromXml: function(xml){
        DwenguinoBlockly.workspace.clear();
        try {
            var xmlDom = Blockly.Xml.textToDom(xml);
        } catch (e) {
            console.log("invalid xml");
            return;
        }
        Blockly.Xml.domToWorkspace(xmlDom, DwenguinoBlockly.workspace);
    },

    setupEnvironment: function(){
        DwenguinoBlockly.initLanguage();
        DwenguinoBlockly.injectBlockly();
        DwenguinoBlockly.loadBlocks('<xml id="startBlocks" style="display: none">' + document.getElementById('startBlocks').innerHTML + '</xml>');
        DwenguinoBlockly.initDwenguinoBlockly();
        DwenguinoBlockly.doTranslation();
        DwenguinoBlockly.setDifficultyLevel(0);
        DwenguinoBlockly.takeSnapshotOfWorkspace();
        $(window).resize(function(){
            DwenguinoBlockly.onresize();
            Blockly.svgResize(DwenguinoBlockly.workspace);
        });
    },
    tearDownEnvironment: function(){
      // Called from the aruino IDE on close. For now do nothing.
    },
};


$(document).ready(function() {
  DwenguinoBlockly.setupEnvironment();
});
