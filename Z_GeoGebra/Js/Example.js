function ggbOnInit() {
    CreateText("Translation Silder:");
    CreateSilder("Sx", "0", "180", "0.1", 1, false, false, false, false, false);
    CreateSilder("Sy", "0", "180", "0.1", 1, false, false, false, false, false);
    CreateSilder("Sz", "0", "180", "0.1", 1, false, false, false, false, false);
    CreateText("Rotation Silder:");
    CreateSilder("Rx", "0°", "180°", "0.1°", 1, false, false, false, false, false);
    CreateSilder("Ry", "0°", "180°", "0.1°", 1, false, false, false, false, false);
    CreateSilder("Rz", "0°", "180°", "0.1°", 1, false, false, false, false, false);
    CreateText("Translation Silder:");
    CreateSilder("Tx", "0°", "180°", "0.1°", 1, false, false, false, false, false);
    CreateSilder("Ty", "0°", "180°", "0.1°", 1, false, false, false, false, false);
    CreateSilder("Tz", "0°", "180°", "0.1°", 1, false, false, false, false, false);
    CreateText("Model to world matrix:");
    let SM = [
        "Sx", 0, 0, 0,
        0, "Sy", 0, 0,
        0, 0, "Sz", 0,
        0, 0, 0, 1
    ];
    let ScaleMatrix = MakeMatrix_NxN(4, SM);

    let RXM = [
        1, 0, 0, 0,
        0, "cos(Rx)", "-sin(Rx)", 0,
        0, "sin(Rx)", "cos(Rx)", 0,
        0, 0, 0, 1
    ];
    let RYM = [
        "cos(Ry)", 0, "-sin(Ry)", 0,
        0, 1, 0, 0,
        "sin(Ry)", 0, "cos(Ry)", 0,
        0, 0, 0, 1
    ];
    let RZM = [
        1, 0, 0, 0,
        0, "cos(Rz)", "-sin(Rz)", 0,
        0, "sin(Rz)", "cos(Rz)", 0,
        0, 0, 0, 1
    ];
    let RotationMatrix = MakeMatrix_1xN(3,"multi",[MakeMatrix_NxN(4,RXM),MakeMatrix_NxN(4,RYM),MakeMatrix_NxN(4,RZM)]);


    let TM = [
        1, 0, 0, "Tx",
        0, 1, 0, "Ty",
        0, 0, 1, "Tz",
        0, 0, 0, 1
    ];
    let TranslateMatrix = MakeMatrix_NxN(4, TM);
    let M = [ScaleMatrix,RotationMatrix, TranslateMatrix];
    CreateMatrix_NxN(3, "multi", "M", M);
}

function CreateText(name) {
    ggbApplet.evalCommand(`"${name}"`);

}

function CreateBoolean(name) {
    ggbApplet.evalCommand(`${name}=false`);

}

function CreateSilder(name, min, max, increase, speed, board, bool1, bool2, bool3, bool4) {
    ggbApplet.evalCommand(`${name} = Slider(${min},${max},${increase},${speed},${board},${bool1},${bool2},${bool3},${bool4})`);
}
function CreatePoint(name, x, y) {
    ggbApplet.evalCommand(`${name} =(${x},${y})`);
}

function CreateMatrix_NxN(number, type, name, array) {
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

function MakeMatrix_1xN(number, type, array) {
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