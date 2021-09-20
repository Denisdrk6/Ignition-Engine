#pragma once

class Application;
struct PhysBody3D;

class Module
{
private :
	bool enabled;

public:
	Module(bool start_enabled = true) : enabled(start_enabled)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	void Enable() 
	{
		if (enabled == false)
		{
			enabled = true;
			Start();
		}
	}


	void Disable()
	{
		if (enabled == true)
		{
			enabled = false;
			CleanUp();
		}
	}
};