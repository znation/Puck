function Scene()
{
    this.color = "rgb(0,255,255)";
    this.padding = 16;
    this.width = width - (2 * this.padding);
    this.height = height - (2 * this.padding);
    this.position = new b2Vec2(this.padding, this.padding);
    this.puck = new Puck;
    this.players = [new Player(0, this), new Player(1, this)];

    // Construct a border box to keep everything inside
    var edges = [
        { // bottom
            width: width,
            height: this.padding,
            x: 0,
            y: height
        },
        { // top
            width: width,
            height: this.padding,
            x: 0,
            y: 0
        },
        { // left
            width: this.padding,
            height: height,
            x: 0,
            y: 0
        },
        { // right
            width: this.padding,
            height: height,
            x: width,
            y: 0
        }
    ];

    for (var i=0; i<edges.length; i++)
    {
        var edge = edges[i];
        edge.width = pixelToBoxCoords(edge.width);
        edge.height = pixelToBoxCoords(edge.height);
        edge.x = pixelToBoxCoords(edge.x);
        edge.y = pixelToBoxCoords(edge.y);

        var boxSd = new b2BoxDef();
        boxSd.extents.Set(edge.width, edge.height);
        boxSd.density = 0;

        var boxBd = new b2BodyDef();
        boxBd.AddShape(boxSd);
        boxBd.position.Set(edge.x, edge.y);
        
        var body = world.CreateBody(boxBd);
        body.m_userData = {
            type: "wall",
            dir: (i < 2 ? "x" : "y") 
        };
    }
}
Scene.prototype = new Rectangle;
Scene.prototype.draw = function(ctx) {
    this.constructor.prototype.draw.call(this, ctx);
    this.players[0].draw(ctx);
    this.players[1].draw(ctx);
    this.puck.draw(ctx);
};
Scene.prototype.move = function() {
    this.players[0].stick.move();
};

