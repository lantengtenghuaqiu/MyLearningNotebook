//Main logic-------------------------------------------------------------------
Main();
//-----------------------------------------------------------------------------

//Logic function definations---------------------------------------------------
function Main() {
    text_index = [0];
    state = ["Init", "Update"];
    Init(state, text_index);
    Update(state);
}

//When the object is create, it will not be created again if existed.
function Init(state, text_index) {
    if (!ggbApplet.exists(state[0])) {
        ggbApplet.evalCommand(`init="Init"`);
        ggbApplet.setVisible("init", false);

        ggbApplet.evalCommand(`${state[0]}=false`);
    } else if (!ggbApplet.exists(state[1]) && GetValue(state[0])) {
        ggbApplet.evalCommand(`${state[1]}=false`);

        //Lights
        //Directional light
        CreateText("Light Group", text_index);
        MakeModelToWolrdTransform(0, text_index,1,0,10,0);
        CreatePoint(3, "LightPos", 0, 0, 0, null);
        SetVisible("LightPos", false);

        CreatePoint(3, "LightDir", 0, -1, 0, "M_0");
        SetVisible("LightDir", false);

        CreateObjectByExists("LightPos'", "M_0 * LightPos");
        CreateEvalCommand("LightDir' = UnitVector(Vector(LightPos',LightPos'+LightDir))");


        //Objects
        CreateText("Object Group", text_index);
        MakeModelToWolrdTransform(1, text_index,1,0,0,0);

        CreatePoint(3, "OB1", 0, 0, 0, null);
        SetVisible("OB1", false);

        CreatePoint(3, "nOB1", 0, 1, 0, "M_1");
        SetVisible("nOB1", false);

        CreateObjectByExists("OB1'", "M_1 * OB1");
        CreateEvalCommand("nOB1' = UnitVector(Vector(OB1',OB1'+nOB1))");

        CreateText("Distance of Light to OB1", text_index);
        CreateEvalCommand("Dist' = Distance(OB1',LightPos')");

        CreateText("N dot L", text_index);

        CreateEvalCommand("ndl' = Dot(nOB1,-LightDir)");
        
    }
}

function Update(state) {

    // SetColor("OB1'",c,c,c);
    if (ggbApplet.exists(`${state[1]}`) && GetValue(`${state[1]}`)) {
        ndl = Clamp((GetValue("ndl'")) * 255,0,255);
        c = ndl;
        SetColor("OB1'",c,c,c);
    }

}

function Events() {

}
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
function MakeModelToWolrdTransform(index, text_index,scale,tx,ty,tz) {
    CreateText("Translation Silder:", text_index);
    CreateSilder(false, `Sx_${index}`, -100, 100, "0.1", 1, false, false, false, false, false);
    ggbApplet.setValue(`Sx_${index}`, scale);
    CreateSilder(false, `Sy_${index}`, -100, 100, "0.1", 1, false, false, false, false, false);
    ggbApplet.setValue(`Sy_${index}`, scale);
    CreateSilder(false, `Sz_${index}`, -100, 100, "0.1", 1, false, false, false, false, false);
    ggbApplet.setValue(`Sz_${index}`, scale);


    CreateText("Rotation Silder:", text_index);
    CreateSilder(false, `Rx_${index}`, "0°", "360°", "0.1°", 1, false, false, false, false, false);
    CreateSilder(false, `Ry_${index}`, "0°", "360°", "0.1°", 1, false, false, false, false, false);
    CreateSilder(false, `Rz_${index}`, "0°", "360°", "0.1°", 1, false, false, false, false, false);

    CreateText("Translation Silder:", text_index);
    CreateSilder(false, `Tx_${index}`, -100, 100, "0.1", 1, false, false, false, false, false);
    ggbApplet.setValue(`Tx_${index}`, tx);
    CreateSilder(false, `Ty_${index}`, -100, 100, "0.1", 1, false, false, false, false, false);
    ggbApplet.setValue(`Ty_${index}`, ty);
    CreateSilder(false, `Tz_${index}`, -100, 100, "0.1", 1, false, false, false, false, false);
    ggbApplet.setValue(`Tz_${index}`, tz);

    CreateText("Model to world matrix:", text_index);

    let SM = [
        `Sx_${index}`, 0, 0, 0,
        0, `Sy_${index}`, 0, 0,
        0, 0, `Sz_${index}`, 0,
        0, 0, 0, 1
    ];
    let ScaleMatrix = MakeMatrix_NxN(4, SM);

    let RXM = [
        1, 0, 0, 0,
        0, `cos(Rx_${index})`, `-sin(Rx_${index})`, 0,
        0, `sin(Rx_${index})`, `cos(Rx_${index})`, 0,
        0, 0, 0, 1
    ];
    let RYM = [
        `cos(Ry_${index})`, 0, `sin(Ry_${index})`, 0,
        0, 1, 0, 0,
        `-sin(Ry_${index})`, 0, `cos(Ry_${index})`, 0,
        0, 0, 0, 1
    ];
    let RZM = [
        1, 0, 0, 0,
        0, `cos(Rz_${index})`, `-sin(Rz_${index})`, 0,
        0, `sin(Rz_${index})`, `cos(Rz_${index})`, 0,
        0, 0, 0, 1
    ];
    let RotationMatrix = CombineMatrices_1xN(3, "multi", [MakeMatrix_NxN(4, RXM), MakeMatrix_NxN(4, RYM), MakeMatrix_NxN(4, RZM)]);


    let TM = [
        1, 0, 0, `Tx_${index}`,
        0, 1, 0, `Ty_${index}`,
        0, 0, 1, `Tz_${index}`,
        0, 0, 0, 1
    ];

    let TranslateMatrix = MakeMatrix_NxN(4, TM);
    let M = [TranslateMatrix, RotationMatrix, ScaleMatrix];
    CreateMatrix_NxN(3, "multi", `M_${index}`, M);



}

//Utility functions-------------------------------------------------
//Matrices
function CreateMatrix_NxN(number, type, name, array) {
    if (!ggbApplet.exists(`${name}`)) {
        if (Array.isArray(array)) {
            if (type == "create") {
                switch (number) {
                    case 2:
                        ggbApplet.evalCommand(`${name} = {{${array[0]}, ${array[1]}},{${array[2]},${array[3]}}}`)
                        break;
                    case 3:
                        ggbApplet.evalCommand(`${name} = {{${array[0]},${array[1]},${array[2]}},{${array[3]},${array[4]},${array[5]}},{${array[6]},${array[7]},${array[8]}}}`)
                        break;
                    case 4:
                        ggbApplet.evalCommand(`${name} = {{${array[0]},${array[1]},${array[2]},${array[3]}},{${array[4]},${array[5]},${array[6]},${array[7]}},{${array[8]},${array[9]},${array[10]},${array[11]}}}`)
                        break;
                    default:
                        break;
                }
            } else if (type == "multi") {
                switch (number) {
                    case 2:
                        ggbApplet.evalCommand(`${name} = ${array[0]}*${array[1]}`);
                        break;
                    case 3:
                        ggbApplet.evalCommand(`${name} = ${array[0]}*${array[1]}*${array[2]}`);
                        break;
                    case 4:
                        ggbApplet.evalCommand(`${name} = ${array[0]}*${array[1]}*${array[2]}*${array[3]}`);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

function CombineMatrices_1xN(number, type, array) {
    if (Array.isArray(array)) {
        if (type == "create") {
            switch (number) {
                case 2:
                    return `{{${array[0]}, ${array[1]}}}`;
                case 3:
                    return `{{${array[0]},${array[1]},${array[2]}}}`;
                case 4:
                    return `{{${array[0]},${array[1]},${array[2]},${array[3]}}}`;
                default:
                    break;
            }
        } else if (type == "multi") {
            switch (number) {
                case 2:
                    return `${array[0]}*${array[1]}`;
                case 3:
                    return `${array[0]}*${array[1]}*${array[2]}`;
                case 4:
                    return `${array[0]} * ${array[1]} * ${array[2]} * ${array[3]}`;
                default:
                    break;
            }
        }
    }
}

function MakeMatrix_NxN(number, array) {
    if (Array.isArray(array)) {
        switch (number) {
            case 2:
                return `{{${array[0], array[1]}},{${array[2]},${array[3]}}}`;
            case 3:
                return `{{${array[0]},${array[1]},${array[2]}},{${array[3]},${array[4]},${array[5]}},{${array[6]},${array[7]},${array[8]}}}`;
            case 4:
                return `{{${array[0]},${array[1]},${array[2]},${array[3]}},{${array[4]},${array[5]},${array[6]},${array[7]}},{${array[8]},${array[9]},${array[10]},${array[11]}},{${array[12]},${array[13]},${array[14]},${array[15]}}}`;
            default:
                break;
        }
    }
}


function Debug() {
    ggbApplet.evalCommand("DEBUGER=0");
}

//Create Object
function CreateEvalCommand(contents) {
    ggbApplet.evalCommand(contents);
}

function CreateText(contents, text_index) {

    if (Array.isArray(text_index)) {
        if (!ggbApplet.exists(`t${text_index[0]}`)) {
            ggbApplet.evalCommand(`t${text_index[0]}="${contents}"`);
            ggbApplet.setVisible(`t${text_index[0]}`, false);
            text_index[0] = text_index[0] + 1;
        }
    } else {
        ggbApplet.evalCommand(`"(CreateText(..,!):Not array"`);

    }


}

function CreateBoolean(name) {
    if (!ggbApplet.exists(`${name}`))
        ggbApplet.evalCommand(`${name}=false`);

}

function CreateSilder(play, name, min, max, increase, speed, board, bool1, bool2, bool3, bool4) {
    if (!ggbApplet.exists(`${name}`)) {
        ggbApplet.evalCommand(`${name} = Slider(${min},${max},${increase},${speed},${board},${bool1},${bool2},${bool3},${bool4})`);
        ggbApplet.setAnimating(`${name}`, play);
    }
}

function CreatePoint(coord, name, x, y, z, matrix) {
    if (!ggbApplet.exists(`${name}`)) {

        if (matrix == null) {
            if (coord == 2)
                ggbApplet.evalCommand(`${name} =(${x},${y})`);
            if (coord == 3)
                ggbApplet.evalCommand(`${name} =(${x},${y},${z})`);
        } else {
            if (ggbApplet.exists(`${matrix}`)) {
                if (coord == 2)
                    ggbApplet.evalCommand(`${name} = ${matrix} (${x},${y})`);
                if (coord == 3)
                    ggbApplet.evalCommand(`${name} =${matrix} (${x},${y},${z})`);
            }

        }

    }
}

function CreateObjectByExists(name, object) {
    if (!ggbApplet.exists(`${name}`)) {
        ggbApplet.evalCommand(`${name}=${object}`);
    }
}

//Get value of object
function GetValue(name) {
    if (ggbApplet.exists(`${name}`)) {
        return ggbApplet.getValue(`${name}`);
    }
}
function GetCoords(name, coord) {
    if (ggbApplet.exists(`${name}`)) {
        switch (coord) {
            case 'x':
                return ggbApplet.getXcoord(`${name}`);
            case 'y':
                return ggbApplet.getYcoord(`${name}`);
            case 'z':
                return ggbApplet.getZcoord(`${name}`);
        }
    }
}
//Set objects
function SetVisible(name, visible) {
    if (ggbApplet.exists(`${name}`)) {
        ggbApplet.setVisible(`${name}`, visible);
    }
}

function SetColor(name, r, g, b) {
    if (ggbApplet.exists(`${name}`))
        ggbApplet.setColor(`${name}`, r, g, b);

}

//Math
function Dot(vector1, vector2, text_index) {
    if (Array.isArray(vector1) && Array.isArray(vector2) && vector1.length == vector2.length) {
        result = 0.0;
        for (i = 0; i < vector1.length; i++) {
            result += vector1[i] * vector2[i];
        }
        return result;
    } else {
        CreateText("(Dot(vector1 , vector2))Wrong creation", text_index);
    }
}

function Ceil(value){
    return value - (value % 1);
}

function Clamp(value, min, max) {
    if (value <= min)
        value = min;
    if (value >= max)
        value = max;
    return value;
}
//---------------------------------------------------------------------