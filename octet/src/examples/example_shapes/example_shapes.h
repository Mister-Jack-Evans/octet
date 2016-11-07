////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
namespace octet {
  /// Scene containing a box with octet.
  class example_shapes : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;

	int numPlanks = 5;
	btRigidBody *rb_bridge[5];
	 
	mat4t mat;

	material *red;
	material *green;
	material *blue;
	material *yellow;
	material *black;
	material *white;

  public:
    example_shapes(int argc, char **argv) : app(argc, argv) {
    }

    ~example_shapes() {
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();
      app_scene->get_camera_instance(0)->get_node()->translate(vec3(0, 4, numPlanks*3));

	  red = new material(vec4((1.0f, 0.0f, 0.0f, 1.0f)));
	  green = new material(vec4((0.0f, 1.0f, 0.0f, 1.0f)));
	  blue = new material(vec4((0.0f, 0.0f, 1.0f, 1.0f)));
	  yellow = new material(vec4((1, 1, 0, 1)));
	  black = new material(vec4((1.0f, 1.0f, 1.0f, 1.0f)));
	  white = new material(vec4((0.0f, 0.0f, 0.0f, 1.0f)));

	  //West Side of bridge
	  mat.loadIdentity();
	  mat.translate(-14, 0, 0);
	  mesh_instance *west_side = app_scene->add_shape(mat, new mesh_box(vec3(1, 1, 20), 1), green, false);
	  rb_bridge[0] = west_side->get_node()->get_rigid_body();

	  //Run through and draw planks of bridge
	  for (int i = 1; i < numPlanks - 1; i++) {
		  mat.loadIdentity();
		  mat.translate(-12.0f + (i * 2), 0.0f, 0.0f);
		  mesh_instance *m = app_scene->add_shape(mat, new mesh_box(vec3(0.8f, 0.1f, 10), 1), yellow, false);
		  rb_bridge[i] = m->get_node()->get_rigid_body();
	  }

	  //East Side of bridge
	  mat.loadIdentity();
	  mat.translate(10, 0, 0);
	  mesh_instance *east_side = app_scene->add_shape(mat, new mesh_box(vec3(1, 1, 20), 1), green, false);
	  rb_bridge[numPlanks - 1] = west_side->get_node()->get_rigid_body();

	  //Constraints
	  btTransform tran1A = btTransform::getIdentity();
	  tran1A.setOrigin(btVector3(0.8f, 0.0f, 18.0f));

	  btTransform tran1B = btTransform::getIdentity();
	  tran1B.setOrigin(btVector3(-0.8f, 0.0f, 18.0f));

	  btTransform tran2A = btTransform::getIdentity();
	  tran2A.setOrigin(btVector3(0.8f, 0.0f, -18.0f));

	  btTransform tran2B = btTransform::getIdentity();
	  tran2B.setOrigin(btVector3(-0.8f, 0.0f, -18.0f));

	  //Apply the spring to the rb_bridge array populated above

	  for (int i = 0; i < numPlanks -1 ; i++) {
		  app_scene->addSpring(rb_bridge[i], rb_bridge[i + 1], tran1A, tran1B);
		  app_scene->addSpring(rb_bridge[i], rb_bridge[i + 1], tran2A, tran2B);
	  }

	  

	  //draw the water under the bridge
	  mat.loadIdentity();
	  mat.translate(0, -10, 0);
	  app_scene->add_shape(mat, new mesh_box(vec3(28, 1, numPlanks*3), 1), blue, false);

    }
	
	

	void spawnPlayer() {
		if (is_key_down(key_S) || is_key_down(key_s)) {
			mat.loadIdentity();
			mat.translate(11, 2, 0);
			app_scene->add_shape(mat, new mesh_sphere(vec3(0.5f, 0.5f, 0.5f), 1), black, true);
		}
	}



    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
		
		//move_Ball();
		
		spawnPlayer();

		//moveSwing();
		
		int vx = 0, vy = 0;
		get_viewport_size(vx, vy);
		app_scene->begin_render(vx, vy);

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);

      // draw the scene
      app_scene->render((float)vx / vy);
    }
  };
}
