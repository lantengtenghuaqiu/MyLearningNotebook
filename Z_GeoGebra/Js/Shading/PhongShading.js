//GLOBAL------------------------------------------------------------------
STATIC = { text_index: [0], p: 0, LightColor: [210, 120, 0], l: [0, 0, 0], n: [0, 0, 0] }

//------------------------------------------------------------------------




//Main logic--------------------------------------------------------------
Main();
//------------------------------------------------------------------------

//Logic function definitions-----------------------------------------------
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
    } else if (ggbApplet.exists(state[0]) && GetValue(state[0])) {
        ggbApplet.evalCommand(`${state[1]}=false`);
        ggbApplet.evalCommand(`${state[2]}=false`);
        //Init your objects-----------------------------------------------
        CreateText("Sphere")
        for (var i = 30; i < 180; i += 30) {
            for (k = 0; k < 360; k += 30) {
                ggbApplet.evalCommand(`P${STATIC.p}=(2*cos(${k}°)*sin(${i}°) , 2*sin(${k}°)*sin(${i}°) , 2*cos(${i}°))`);
                ggbApplet.evalCommand(`V${STATIC.p}=UnitVector(Vector(P${STATIC.p}, P${STATIC.p}+P${STATIC.p}))`);
                STATIC.p += 1;
            }
        }

        for (var i = 0; i < STATIC.p; i++) {
            ggbApplet.setLabelVisible(`P${i}`, false);
            ggbApplet.setPointSize(`P${i}`, 4);
            ggbApplet.setLineThickness(`V${i}`, 2);
            // SetVisible(`V${i}`,false);
        }
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


        //----------------------------------------------------------------
        ggbApplet.evalCommand(`p=${STATIC.p}`);
        ggbApplet.evalCommand(`${state[0]} = false`);
    }
}

function Update(state) {
    if (ggbApplet.exists(`${state[1]}`) && !GetValue(`${state[1]}`)) {
    }
    else if (ggbApplet.exists(`${state[1]}`)) {
        //Update logic----------------------------------------------------
        p = GetValue("p");

        l = [0, 0, 0];
        l[0] = GetCoords("LightDir'", 'x');
        l[1] = GetCoords("LightDir'", 'y');
        l[2] = GetCoords("LightDir'", 'z');
        n = [0, 0, 0];
        for (i = 0; i < p; i++) {
            n[0] = GetCoords(`V${i}`, 'x');
            n[1] = GetCoords(`V${i}`, 'y');
            n[2] = GetCoords(`V${i}`, 'z');
            ndl = -(n[0] * l[0] + n[1] * l[1] + n[2] * l[2]);
            c =
                [
                    ndl * 240,
                    ndl * 225,
                    ndl * 200
                ];
            if (c[0] >= 255) 
                c[0] = 255;
            if (c[0] < 0) 
                c[0] = 0;

            if (c[1] >= 255) 
                c[1] = 255;
            if (c[1] < 0) 
                c[1] = 0;

            if (c[2] >= 255) 
                c[2] = 255;
            if (c[2] < 0) 
                c[2] = 0;

            SetColor(`P${i}`, c[0], c[1], c[2]);
            SetColor(`V${i}`, c[0], c[1], c[2]);

        }


        //----------------------------------------------------------------
    }
}

function Events() {

}

function Destory(state) {
    if (ggbApplet.exists(`${state[2]}`) && ggbApplet.getValue(`${state[2]}`)) {
        ggbApplet.setValue(`${state[0]}`, false);
        ggbApplet.setValue(`${state[1]}`, false);
        ggbApplet.setValue(`${state[2]}`, false);
        allnames = [];
        allnames = ggbApplet.getAllObjectNames();
        for (i = allnames.length - 1; i > 4; i--) {
            if (ggbApplet.exists(`${allnames[i]}`))
                ggbApplet.deleteObject(allnames[i]);
        }
        ggbApplet.stopAnimation();

    }
}
//------------------------------------------------------------------------



//Special Creations-------------------------------------------------------
//Model to world transform matrix
function MakeModelToWolrdTransform(index, scale, tx, ty, tz) {
    CreateText("Translation Silder:");
    CreateSilder(false, `Sx_${index}`, -100, 100, "0.1", 1, 1, scale);
    CreateSilder(false, `Sy_${index}`, -100, 100, "0.1", 1, 1, scale);
    CreateSilder(false, `Sz_${index}`, -100, 100, "0.1", 1, 1, scale);


    CreateText("Rotation Silder:");
    CreateSilder(false, `Rx_${index}`, "0°", "360°", "0.1°", 1, 1);
    CreateSilder(false, `Ry_${index}`, "0°", "360°", "0.1°", 1, 1);
    CreateSilder(false, `Rz_${index}`, "0°", "360°", "0.1°", 1, 1);

    CreateText("Translation Silder:");
    CreateSilder(false, `Tx_${index}`, -100, 100, "0.1", 1, 1, tx);
    CreateSilder(false, `Ty_${index}`, -100, 100, "0.1", 1, 1, ty);
    CreateSilder(false, `Tz_${index}`, -100, 100, "0.1", 1, 1, tz);

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
//------------------------------------------------------------------------

//Utility functions-------------------------------------------------------
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
    if (Array.isArray(STATIC.text_index)) {
        if (!ggbApplet.exists(`t${STATIC.text_index[0]}`)) {
            ggbApplet.evalCommand(`t${STATIC.text_index[0]}="${contents}"`);
            ggbApplet.setVisible(`t${STATIC.text_index[0]}`, false);
            STATIC.text_index[0] = STATIC.text_index[0] + 1;
        }
    } else {
        ggbApplet.evalCommand(`"(CreateText(..,!):Not array"`);

    }


}

function CreateBoolean(name) {
    if (!ggbApplet.exists(`${name}`))
        ggbApplet.evalCommand(`${name}=false`);

}

function CreateSilder(isAngle, name, min, max, increase, speed, board, inital = 0, bool1 = false, bool2 = false, bool3 = false, bool4 = false,) {
    if (!ggbApplet.exists(`${name}`)) {
        if (!isAngle) {
            ggbApplet.evalCommand(`${name} = Slider(${min},${max},${increase},${speed},${board},${bool1},${bool2},${bool3},${bool4})`);
            ggbApplet.setValue(`${name}`, inital);
        } else {
            ggbApplet.evalCommand(`${name} = Slider(${min}°,${max}°,${increase}°,${speed},${board},${bool1},${bool2},${bool3},${bool4})`);
            ggbApplet.setValue(`${name}`, inital);
        }
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

function CreateSphere(name, center, radius) {
    ggbApplet.evalCommand(`${name} : (x-x(${center}))^2 + (y-y(${center}))^2 + (z-z(${center}))^2 = ${radius}`);
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
function SetValue(name, value) {
    if (ggbApplet.exists(`${name}`)) {
        ggbApplet.setValue(`${name}`, value);
    }
}
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

//Math
function Dot(vector1, vector2) {
    if (Array.isArray(vector1) && Array.isArray(vector2) && vector1.length == vector2.length) {
        result = 0.0;
        for (var i = 0; i < vector1.length; i++) {
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
        for (var i = 0; i < value.length; i++) {
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

function Pow(value, times) {
    result = 1.0;
    for (var i = 0; i < times; i++) {
        result *= value;
    }
    return result;
}

function Abs(value) {
    if (value < 0)
        value *= -1;
    return value;
}

function Max(value, max) {
    if (value < max)
        return max;
    return value;
}
//------------------------------------------------------------------------