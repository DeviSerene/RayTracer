#include "fps.h"
#include <math.h>

Fps::Fps()
{

	memset(m_frameTimes, 0, sizeof(m_frameTimes));
	m_frameTimeLast = SDL_GetTicks();
	// total frames rendered
	m_frameCount = 0;
	m_framesPerSecond = 0;
	m_fpsDisplay = { 0,40,45,15 };
	//
	m_digitTens = { 50,40,15,15 };
	m_digitOnes = { 66,40,15,15 };
	m_digitPoint = { 82,40,15,15 };
	m_digitTenth = { 98,40,15,15 };
	m_digitHundreth = { 114,40,15,15 };
	m_digitThousanth = { 130,40,15,15 };
}

Fps::~Fps()
{

}

void Fps::Update(float _currentTime)
{

	//FPS
	// frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
	// This value rotates back to 0 after it hits FRAME_VALUES.
	m_frameTimesIndex = m_frameCount % FRAME_VALUES;
	// store the current time
	// save the frame time value
	m_frameTimes[m_frameTimesIndex] = _currentTime - m_frameTimeLast;
	// save the last frame time for the next fpsthink
	m_frameTimeLast = _currentTime;
	// increment the frame count
	m_frameCount++;
	if (m_frameCount < FRAME_VALUES) { m_count = m_frameCount; }
	else { m_count = FRAME_VALUES; }
	// add up all the values and divide to get the average frame time.
	m_framesPerSecond = 0;
	for (int i = 0; i < m_count; i++) { m_framesPerSecond += m_frameTimes[i]; }
	m_framesPerSecond /= m_count;
	// now to make it an actual frames per second value...
	m_framesPerSecond = 1000.f / m_framesPerSecond;
	
	
}

void Fps::Draw()
{
	if (!std::isinf(m_framesPerSecond))
	{
	//	std::cout << "FPS: " << m_framesPerSecond << std::endl;
		SpriteFactory::Draw("assets/fps.png", m_fpsDisplay);
		//SpriteFactory::Draw("assets/p.png", m_digitPoint);
		m_fpsS = std::to_string(m_framesPerSecond);
		for (int i = 0; i < m_fpsS.size(); i++)
		{
			m_writer = "assets/";
			m_writer += m_fpsS.at(i);
			m_writer += ".png";
			switch (i)
			{
			case 0:
				SpriteFactory::Draw(m_writer, m_digitTens);
				break;
			case 1:
				SpriteFactory::Draw(m_writer, m_digitOnes);
				break;
			case 2:
				SpriteFactory::Draw(m_writer, m_digitPoint);
				break;
			case 3:
				SpriteFactory::Draw(m_writer, m_digitTenth);
				break;
			case 4:
				SpriteFactory::Draw(m_writer, m_digitHundreth);
				break;
			case 5:
				SpriteFactory::Draw(m_writer, m_digitThousanth);
				break;
			}
			if (i >= 5)
				break;
		}
		//SpriteFactory::Draw("assets/"".png", m_digitTens);
	}
}