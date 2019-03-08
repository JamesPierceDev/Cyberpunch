#include "..\Header\InputSystem.h"

void InputSystem::addComponent(Component * component)
{
	m_components.push_back(component);
}

void InputSystem::update(double dt)
{	
	SDL_PumpEvents(); //Update the keystate

	//Update all components
	for (auto& comp : m_components)
	{
		//Convert the component to an input component
		auto input = static_cast<InputComponent*>(comp);

		input->m_previous = input->m_current; //Set the previous

		//Check if the keys are currently down
		input->m_current["W"] = input->m_keyStates[SDL_SCANCODE_W];
		input->m_current["A"] = input->m_keyStates[SDL_SCANCODE_A];
		input->m_current["S"] = input->m_keyStates[SDL_SCANCODE_S];
		input->m_current["D"] = input->m_keyStates[SDL_SCANCODE_D];
		input->m_current["C"] = input->m_keyStates[SDL_SCANCODE_C];

		//Check joycon buttons
		input->m_current["ABTN"] = SDL_JoystickGetButton(input->m_joycon, 0);
		input->m_current["XBTN"] = SDL_JoystickGetButton(input->m_joycon, 1);
		input->m_current["BBTN"] = SDL_JoystickGetButton(input->m_joycon, 2);
		input->m_current["YBTN"] = SDL_JoystickGetButton(input->m_joycon, 3);
		input->m_current["LBBTN"] = SDL_JoystickGetButton(input->m_joycon, 4);
		input->m_current["RBBTN"] = SDL_JoystickGetButton(input->m_joycon, 5);
		input->m_current["MINUS"] = SDL_JoystickGetButton(input->m_joycon, 8);
		input->m_current["PLUS"] = SDL_JoystickGetButton(input->m_joycon, 9);

		//Determinging the status of the sticks
		input->m_current["STICKUP"] = SDL_JoystickGetHat(input->m_joycon, 0) == 1 ? true : false;
		input->m_current["STICKDOWN"] = SDL_JoystickGetHat(input->m_joycon, 0) == 4 ? true : false;
		input->m_current["STICKLEFT"] = SDL_JoystickGetHat(input->m_joycon, 0) == 8 ? true : false;
		input->m_current["STICKRIGHT"] = SDL_JoystickGetHat(input->m_joycon, 0) == 2 ? true : false;
		input->m_current["STICKDOWNLEFT"] = SDL_JoystickGetHat(input->m_joycon, 0) == 12 ? true : false;
		input->m_current["STICKDOWNRIGHT"] = SDL_JoystickGetHat(input->m_joycon, 0) == 6 ? true : false;
		input->m_current["STICKUPLEFT"] = SDL_JoystickGetHat(input->m_joycon, 0) == 9 ? true : false;
		input->m_current["STICKUPRIGHT"] = SDL_JoystickGetHat(input->m_joycon, 0) == 3 ? true : false;

		if (SDL_JoystickGetAxis(input->m_joycon, 0) > 30000)
		{
			input->m_current["STICKRIGHT"] = true;
		}
		if (SDL_JoystickGetAxis(input->m_joycon, 0) < -30000)
		{
			input->m_current["STICKLEFT"] = true;
		}
		if (SDL_JoystickGetAxis(input->m_joycon, 1) < -30000)
		{
			input->m_current["STICKUP"] = true;
		}
		if (SDL_JoystickGetAxis(input->m_joycon, 1) > 30000)
		{
			input->m_current["STICKDOWN"] = true;
		}


		/*for (int i = 0; i < 16; i++)
		{
			if (SDL_JoystickGetAxis(input->m_joycon, 1))
			{
				std::cout << "Detected joystick movement with number " << i << "\n";
			}
		}*/
	}
}