//Global----------------------------------------------------------------------
text_index = [0];
OBJECTS = [];
LightColor = [220, 110, 30];



//Main logic------------------------------------------------------------------
Main();
//----------------------------------------------------------------------------

//Logic function definitions---------------------------------------------------
function Main() {
    state = ["Init", "Update", "Destory"];
    Init(state);
    Update(state);
    Destory(state);
}

//When the object is create, it will not be created again if existed.
function Init(state) {
    if (!ggbApplet.exists(state[0])) {
        ggbApplet.evalCommand(`init="Init"`);
        ggbApplet.setVisible("init", false);
        ggbApplet.evalCommand(`${state[0]}=false`);
    } else if (GetValue(state[0])) {
        ggbApplet.evalCommand(`${state[1]}=false`);
        ggbApplet.evalCommand(`${state[2]}=false`);

        //Lights
        DirectionOrPuntual = "DirectionOrPuntual"
        CreateBoolean(DirectionOrPuntual);

        CreateText("Light Group");
        MakeModelToWolrdTransform(0, 1, 0, 10, 0);
        LightPos = "LightPos";
        CreatePoint(3, LightPos, 0, 0, 0, null);
        SetVisible(LightPos, false);
        SetAuxiliary(LightPos, true);

        LightDir = "LightDir";
        CreatePoint(3, LightDir, 0, -1, 0, "M_0");
        SetVisible(LightDir, false);
        SetAuxiliary(LightDir, true);

        LightPosprime = "LightPos'";
        CreateObjectByExists(`${LightPosprime}`, "M_0 * LightPos");
        LightDirprime = "LightDir'";
        CreateEvalCommand(`${LightDirprime} = UnitVector(Vector(LightPos',LightPos'+LightDir))`);
        SetVisible(LightDirprime, DirectionOrPuntual);
        SetAuxiliary(LightDirprime, !DirectionOrPuntual);

        //Objects
        CreateText("Object Group");
        MakeModelToWolrdTransform(1, 1, 0, 0, 0);

        const OB1 = "OB1"
        CreatePoint(3, OB1, 0, 0, 0, null);
        SetVisible(OB1, false);
        SetAuxiliary(OB1, true);

        const nOB1 = "nOB1"
        CreatePoint(3, nOB1, 0, 1, 0, "M_1");
        SetVisible(nOB1, false);
        SetAuxiliary(nOB1, true);



        const OB1prime = "OB1'"
        CreateObjectByExists(OB1prime, "M_1 * OB1");
        SetSize("OB1'", 9);

        const nOB1prime = "nOB1'"
        CreateEvalCommand(`${nOB1prime} = UnitVector(Vector(OB1',OB1'+nOB1))`);


        CreateText("Distance of Light to OB1");

        const distance = "distance";
        CreateEvalCommand(`${distance} = Distance(OB1',LightPos')`);

        CreateText("Light effects");
        CreateText("Normal dot light");
        const ndl = "ndl";
        CreateEvalCommand(`${ndl} = Dot(nOB1,-LightDir)`);
        CreateText("Attenuation");
        const r0 = "r0";
        CreateSilder(false, r0, 0, 10, 1, 0.1, 0, false, false, false, false, 10);
        SetValue("Init",false);
    }
}

function Update(state) {

    if (ggbApplet.exists(`${state[1]}`) && !GetValue(`${state[1]}`)) {
        ggbApplet.evalCommand(`DEBUGing=${OBJECTS.length}`);
    }
    else if (ggbApplet.exists(`${state[1]}`)) {
        direction_punctual = GetValue("DirectionOrPuntual");
        if (direction_punctual) {
            ndl = GetValue("ndl");
            SetVisible("LightDir'", true);
            SetAuxiliary("LightDir'", false);
        } else {
            ndl = 1;
            SetVisible("LightDir'", false);
            SetAuxiliary("LightDir'", true);
        }
        r0 = GetValue("r0");
        r = GetValue("distance");
        attenuation = (r0 * r0) / ((r * r) + 0.001);
        c = Clamp(
            [
                ndl * LightColor[0] * attenuation,
                ndl * LightColor[1] * attenuation,
                ndl * LightColor[2] * attenuation
            ], 0, 255);
        SetColor("OB1'", c[0], c[1], c[2]);
    }

}

function Events() {

}

function Destory(state) {
    if (ggbApplet.exists(`${state[2]}`) && ggbApplet.getValue(`${state[2]}`)) {
        ggbApplet.setValue(`${state[1]}`, false);
        ggbApplet.setValue(`${state[2]}`, false);


        for (i = 0; i < OBJECTS.length; i++) {
            if (ggbApplet.exists(`${OBJECTS[i]}`))
                ggbApplet.deleteObject(`${OBJECTS[i]}`);
        }
    }



}
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
function MakeModelToWolrdTransform(index, scale, tx, ty, tz) {
    CreateText("Translation Silder:");
    CreateSilder(false, `Sx_${index}`, -100, 100, "0.1", 1, false, false, false, false, false, scale);
    CreateSilder(false, `Sy_${index}`, -100, 100, "0.1", 1, false, false, false, false, false, scale);
    CreateSilder(false, `Sz_${index}`, -100, 100, "0.1", 1, false, false, false, false, false, scale);


    CreateText("Rotation Silder:");
    CreateSilder(false, `Rx_${index}`, "0°", "360°", "0.1°", 1, false, false, false, false, false);
    CreateSilder(false, `Ry_${index}`, "0°", "360°", "0.1°", 1, false, false, false, false, false);
    CreateSilder(false, `Rz_${index}`, "0°", "360°", "0.1°", 1, false, false, false, false, false);

    CreateText("Translation Silder:");
    CreateSilder(false, `Tx_${index}`, -100, 100, "0.1", 1, false, false, false, false, false, tx);
    CreateSilder(false, `Ty_${index}`, -100, 100, "0.1", 1, false, false, false, false, false, ty);
    CreateSilder(false, `Tz_${index}`, -100, 100, "0.1", 1, false, false, false, false, false, tz);

    CreateText("Model to world matrix:");

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
                        ggbApplet.evalCommand(`${name} = {{${array[0]}, ${array[1]}},{${array[2]},${array[3]}}}`);
                        break;
                    case 3:
                        ggbApplet.evalCommand(`${name} = {{${array[0]},${array[1]},${array[2]}},{${array[3]},${array[4]},${array[5]}},{${array[6]},${array[7]},${array[8]}}}`);
                        break;
                    case 4:
                        ggbApplet.evalCommand(`${name} = {{${array[0]},${array[1]},${array[2]},${array[3]}},{${array[4]},${array[5]},${array[6]},${array[7]}},{${array[8]},${array[9]},${array[10]},${array[11]}}}`);
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
    else
        OBJECTS.push(name);

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

function CreateText(contents) {
    if (Array.isArray(text_index)) {
        if (!ggbApplet.exists(`t${text_index[0]}`)) {
            ggbApplet.evalCommand(`t${text_index[0]}="${contents}"`);
            ggbApplet.setVisible(`t${text_index[0]}`, false);
            text_index[0] = text_index[0] + 1;
        }
        else
            OBJECTS.push(`t${text_index}`);
    } else {
        ggbApplet.evalCommand(`"(CreateText(..,!):Not array"`);

    }


}

function CreateBoolean(name) {
    if (!ggbApplet.exists(`${name}`))
        ggbApplet.evalCommand(`${name}=false`);
    else
        OBJECTS.push(name);

}

function CreateSilder(play, name, min, max, increase, speed, board, bool1 = false, bool2 = false, bool3 = false, bool4 = false, inital = 0) {
    if (!ggbApplet.exists(`${name}`)) {
        ggbApplet.evalCommand(`${name} = Slider(${min},${max},${increase},${speed},${board},${bool1},${bool2},${bool3},${bool4})`);
        ggbApplet.setAnimating(`${name}`, play);
        ggbApplet.setValue(`${name}`, inital);
    }
    else
        OBJECTS.push(name);

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
    else
        OBJECTS.push(name);

}

function CreateObjectByExists(name, object) {
    if (!ggbApplet.exists(`${name}`)) {
        ggbApplet.evalCommand(`${name}=${object}`);
    }
    else
        OBJECTS.push(name);
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

function SetSize(name, size) {
    if (ggbApplet.exists(`${name}`) && size >= 0) {
        ggbApplet.setPointSize(`${name}`, size);
    }
}

function SetColor(name, r, g, b) {
    if (ggbApplet.exists(`${name}`))
        ggbApplet.setColor(`${name}`, r, g, b);
}

function SetAuxiliary(name, boolean) {
    if (ggbApplet.exists(`${name}`))
        ggbApplet.setAuxiliary(`${name}`, boolean);

}
function SetValue(name,value)
{
    if (ggbApplet.exists(`${name}`))
        ggbApplet.setValue(name,value);
}

//Math
function Dot(vector1, vector2) {
    if (Array.isArray(vector1) && Array.isArray(vector2) && vector1.length == vector2.length) {
        result = 0.0;
        for (i = 0; i < vector1.length; i++) {
            result += vector1[i] * vector2[i];
        }
        return result;
    } else {
        CreateText("(Dot(vector1 , vector2))Wrong creation");
    }
}

function Ceil(value) {
    return value - (value % 1);
}

function Clamp(value, min, max) {
    if (Array.isArray(value)) {
        for (i = 0; i < value.length; i++) {
            if (value[i] <= min)
                value[i] = min;
            if (value[i] >= max)
                value[i] = max;
        }
    } else {
        if (value <= min)
            value = min;
        if (value >= max)
            value = max;
    }

    return value;
}
//---------------------------------------------------------------------