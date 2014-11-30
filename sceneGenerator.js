var fs = require('fs'),
    readline = require('readline'),
    rl = null;

function init() {
    rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout
    });
}

function done() {
    process.exit();
}

function createBox( center, length ) {
    var boxString = "#shape_box 0\n" + 
        center.x + " " + center.y + " " center.z + "\n" +
        length.x + " " length.y + " " + length.z + "\n\n";
}

function generateRayFile( opts ) {
    var chairTemplate =
        "#texture_num 1\n
        #texture ./TestFiles/check.jpg\n
        #material_num 1\n
        #material\n
        0.0 0.0 0.0\n
        0.3 0.3 0.3\n
        0.5 0.1 0.1\n
        1.0 1.0 1.0 100\n
        0.0 0.0 0.0\n
        1.05\n
        -1\n\n";

    // Create base
    var center = {x:0, z:0, y:opts.legHeight};
    chairTemplate += createBox( center, {x:width, z:depth, y:1.0} );

    // Draw legs


    for( var i = 0; i < opts.numTimes; i++ ) {
        // Draw 1 obj
        console.log("Drawing object: " + i);

        // Rotate 90
    }
    main();
}

/** Given params from the user, can generate a chair.ray or table.ray file **/
function main() {
    furnitureType = "NOT_SET",
    numFurniture = 0,
    legHeight = 0, backHeight = 0,
    width = 0, depth = 0;

    rl.on('line', function( line ) {
        if( line == 'quit' ) { done(); }
    });

    /** Prompt the user for various parameters **/
    rl.question("What kind of furniture do you want (chair, table)? ", function(answer) {
        if ( answer.trim().toLowerCase() == "chair" ) {
            furnitureType = "chair";
        } else if( answer.trim().toLowerCase() == "table" ) {
            furnitureType = "table";
        } else {
            console.log( "Only chair and tables are allowed." );
            done();
        }

        switch( furnitureType )
        {
            case "chair":
                rl.question("How many? ", function(answer) {
                    if( isNaN(answer) ) {
                        console.log( "Please enter a valid number." );
                        done();
                    }
                    numFurniture = parseInt(answer);

                    rl.question("How wide should each chair's seat be? ", function(answer) {
                        if ( isNaN( answer )) {
                            console.log( "Please enter a valid number." );
                            done();
                        }
                        width = depth = answer;

                        rl.question("How tall should the back be?", function(answer) {
                            if ( isNaN( answer )) {
                                console.log( "Please enter a valid number." );
                                done();
                            }
                            backHeight = answer;

                            rl.question("How tall should the legs be? ", function(answer) {
                                if ( isNaN( answer )) {
                                    console.log( "Please enter a valid number." );
                                    done();
                                }
                                legHeight = answer;

                                generateRayFile({
                                    "type": furnitureType,
                                    "numTimes": numFurniture,
                                    "legHeight": legHeight,
                                    "backHeight": backHeight,
                                    "width": width,
                                    "depth": depth
                                });
                            });
                        });
                    });
                });
                break;
            case "table":
                break;
            default:
                console.log( "Furniture type unsupported: " + furnitureType );
                break;
        }
    });

    /* Pseudo
     * 1. Prompt user for what type of furniture they want (chair, tabletop) to determine what file to write to.
     * Chair
     *      2a. Ask how big the chair's seat should be
     *      2b. Ask how tall the chair's back should be
     *      2c. Ask how many chairs (<4)
     *      2d. for i=0..numChairs, create them in a rotating fashion
     * 
     * Table
     *      3a. Ask how wide the table should be
     *      3b. Ask how deep the table should be
     *      3c. Ask how tall the table should be
     *      3d. Generate the table at the origin
     */

}
init();
main();
