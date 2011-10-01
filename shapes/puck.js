function Puck()
{
    this.radius = 20;
    this.position = new b2Vec2(
            (width/2),
            (height/2));
    this.color = "rgb(0,255,255)";
    this.maxSpeed = 3;
}
Puck.prototype = new Circle;
Puck.prototype.adjustSpeed = function(factor) {
    var body = this.boxCircle;
    var velocity = body.GetLinearVelocity();
    velocity.Multiply(factor);
};
Puck.prototype.applyConstraints = function() {
    var body = this.boxCircle;
    var velocity = body.GetLinearVelocity();
    var speed = velocity.Length();
    if (speed > this.maxSpeed)
    {
        velocity.Multiply(this.maxSpeed / speed);
    }
};
Puck.prototype.detectCollisions = function() {
    var contactNode = this.boxCircle.GetContactList();
    while (contactNode != null)
    {
        var contact = contactNode.contact;
        
        var otherShape = (contact.m_shape1.m_body == this.boxCircle) ? 
            contact.m_shape2 :
            contact.m_shape1;
        
        var otherBody = otherShape.m_body;

        if (otherBody.m_userData != null &&
                otherBody.m_userData.type == "wall")
        {
            //var rotation = this.boxCircle.GetRotation();

            this.adjustSpeed(0.9);

            if (otherBody.m_userData.dir == "x")
            {
                // collision with floor or ceiling
            }
            else
            {
                // collision with left or right wall
            }
        }

        contactNode = contactNode.next;
    }
};

