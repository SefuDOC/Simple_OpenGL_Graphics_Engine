local speed = 45.0

return {
  Update = function(self, nodeName, dt)
    engine.Rotate(nodeName, 0.0, speed * dt, 0.0)
  end
}