function Puck()
{
    this.radius = 20;
    this.position = new b2Vec2(
            (width/2),
            (height/2));
    this.angle = Math.random() * Math.PI * 2;
    this.speed = 5;
    this.color = "rgb(0,255,255)";
}
Puck.prototype = new Circle;
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
            var rotation = this.boxCircle.GetRotation();

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

