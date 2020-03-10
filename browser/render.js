class Board{
    constructor(maxWH, dim){
        this.maxWH = maxWH;
        this.dim = dim;
        this.offset = -1*dim/2;
        this.boxsize = maxWH/(dim + 1);
        this.move = 0;
    }

    //30 , 30 | 570, 570

    //setup grid and coordinate system
    init(){
        var canvas = document.getElementById('canvas');
        if (canvas.getContext) {
            var board = canvas.getContext('2d');
            board.canvas.width = this.maxWH;
            board.canvas.height = this.maxWH;
            board.fillStyle = '#dec9a0';
            board.fillRect(0, 0, board.canvas.width, board.canvas.height);


            //creates grid
            for (var i = 1; i < this.dim + 1; i++) {
                board.beginPath();
                board.moveTo(i * this.boxsize, this.boxsize);
                board.lineTo(i * this.boxsize, this.maxWH - this.boxsize)
                board.stroke();

                board.beginPath();
                board.moveTo(this.boxsize, i * this.boxsize);
                board.lineTo(this.maxWH - this.boxsize, i * this.boxsize)
                board.stroke();
            }

            //visualize click radius
            for(var i = 1; i<= this.dim; i++){
                for(var j = 1; j <= this.dim; j++){
                    // var x = this.boxsize * i;
                    // var y = this.boxsize * j;
                    // board.beginPath();
                    // board.arc(x, y, this.boxsize/2, 0, 2 * Math.PI);
                    // board.stroke();
                }
            }

            //add labels
            for (var i = 1; i <= this.dim; i++) {
                board.font = "15px Arial";
                var letter = String.fromCharCode(i + 64);
                board.strokeText(letter, this.offset + (i * this.boxsize), -10 + this.maxWH / (this.dim + 5));
                board.strokeText(i, -15 + this.offset + this.maxWH / (this.dim + 5), -this.offset + (i * this.boxsize) );
            }
        }
    }
    //start the game
    play(){
        $("#canvas").click((e) => {
            var x = e.clientX;
            var y = e.clientY;
            var z = this.findCoord(x, y);
            if(z != null){
                this.addStone(0, z[0], z[1]);
            }
        })
    }
    //adds color-stone at x,y 
    addStone(color, x, y) {
        var board = document.getElementById("canvas").getContext("2d");
        board.beginPath();
        board.arc(x, y, this.boxsize/2.2, 0, 2 * Math.PI);
        if (color == 1) {
            board.fillStyle = 'white';
        } else if (color == 0) {
            board.fillStyle = 'black';
        } else { return false; }
        board.fill();
        board.stroke();
    }
    //finds closests valid position from click and return pixel coord
    findCoord(x, y) {
        var xEst = Math.floor(x * 10 / this.boxsize) / 10; var yEst = Math.floor(y * 10 / this.boxsize) / 10;
        var xPred = this.boxsize * Math.round(xEst); var yPred = this.boxsize * Math.round(yEst);
        var xDist = x - xPred; var yDist = y - yPred;
        var distance = Math.sqrt(xDist * xDist + yDist * yDist);
        
        if (distance <= this.boxsize/2) {
            console.log(xPred, yPred);
            return [xPred, yPred]; 
        }
        else { return null }
    }
    load(board){
        for(var i = 0; i<board.length; i++){
            for(var j = 0; j<board.length; j++){
                this.addStone(board[i][j], this.boxsize * (i+1), this.boxsize * (j+1));
            }
        }
    }
}

$(document).ready(() => {
    var board = new Board(600,19);
    board.init();

    var test = Array(19);
    for(var i = 0; i<test.length; i++){
        test[i] = Array(19);
        for(var j = 0; j<test.length; j++){
            test[i][j] = -1;
        }
    }

    test[1][2] = 1;
    test[1][3] = 0;

    board.load(test);
    board.play();

    
})