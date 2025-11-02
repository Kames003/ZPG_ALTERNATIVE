#include "ShaderProgramManager.h"

ShaderProgramManager::ShaderProgramManager(LightManager* lm)
{
	this->lm = lm;
}

void ShaderProgramManager::addShaderProgram(Camera* c, const char* vertexShaderFile, 
                                           const char* fragmentShaderFile)
{
	// Vytvor nový shader program
	ShaderProgram* sp = new ShaderProgram(c);
	sp->compileShaderProgram(vertexShaderFile, fragmentShaderFile);
	
	// Pridaj do zoznamu shaderu
	shaderPrograms.push_back(sp);
	
	// Pripoj kameru ako observer
	c->attach(sp);
}

ShaderProgram* ShaderProgramManager::getShaderProgram(int index)
{
	if (index < 0 || index >= shaderPrograms.size())
	{
		printf("Shader program %d does not exist.\n", index);
		exit(EXIT_FAILURE);
	}
	return this->shaderPrograms[index];
}

// ========================================
// ✅ OPRAVENÉ: Všetky update metódy
// ========================================

void ShaderProgramManager::updateAmbient()
{
	// Pošli ambient hodnotu do všetkých shaderov
	for (ShaderProgram* s : shaderPrograms)
	{
		s->activateShaderProgram();  // ✅ Aktivuj
		s->updateUniform("ambient", lm->ambient);
		s->deactivateShaderProgram();  // ✅ Deaktivuj
	}
}

void ShaderProgramManager::updatePointLights()
{
	for (ShaderProgram* s : shaderPrograms)
	{
		s->activateShaderProgram();  // ✅ Aktivuj RAZ na začiatku

		int size = lm->getPointLightsSize();
		s->updateUniform("numberOfPointLights", size);

		// Pošli všetky bodové svetlá do shader array
		for (int i = 0; i < size; i++)
		{
			PointLight* light = lm->getPointLight(i);
			std::string base = "pointlights[" + std::to_string(i) + "].";

			s->updateUniform((base + "color").c_str(), light->color);
			s->updateUniform((base + "position").c_str(), light->position);
			s->updateUniform((base + "constant").c_str(), light->constant);
			s->updateUniform((base + "linear").c_str(), light->linear);
			s->updateUniform((base + "quadratic").c_str(), light->quadratic);
		}

		s->deactivateShaderProgram();  // ✅ Deaktivuj RAZ na konci
	}
}

void ShaderProgramManager::updateSpotlights()
{
	for (ShaderProgram* s : shaderPrograms)
	{
		s->activateShaderProgram();  // ✅ Aktivuj

		int size = lm->getSpotlightsSize();
		s->updateUniform("numberOfSpotlights", size);

		for (int i = 0; i < size; i++)
		{
			Spotlight* light = lm->getSpotlight(i);
			std::string base = "spotlights[" + std::to_string(i) + "].";

			s->updateUniform((base + "position").c_str(), light->position);
			s->updateUniform((base + "direction").c_str(), light->direction);
			s->updateUniform((base + "color").c_str(), light->color);
			s->updateUniform((base + "constant").c_str(), light->constant);
			s->updateUniform((base + "linear").c_str(), light->linear);
			s->updateUniform((base + "quadratic").c_str(), light->quadratic);
			s->updateUniform((base + "angle").c_str(), light->angle);
		}

		s->deactivateShaderProgram();  // ✅ Deaktivuj
	}
}

void ShaderProgramManager::updateSpotlightCamera()
{
	if (lm->getSpotlightCamera())
	{
		// ✅ Baterka je ZAPNUTÁ - pošli dáta
		SpotlightCamera* light = lm->getSpotlightCamera();

		for (ShaderProgram* s : shaderPrograms)
		{
			s->activateShaderProgram();  // ✅ Aktivuj

			s->updateUniform("spotlightCamera_bool", true);
			s->updateUniform("spotlightCamera.color", light->color);
			s->updateUniform("spotlightCamera.constant", light->constant);
			s->updateUniform("spotlightCamera.linear", light->linear);
			s->updateUniform("spotlightCamera.quadratic", light->quadratic);
			s->updateUniform("spotlightCamera.angle", light->angle);

			s->deactivateShaderProgram();  // ✅ Deaktivuj
		}
	}
	else
	{
		// ✅ Baterka je VYPNUTÁ - pošli false
		for (ShaderProgram* s : shaderPrograms)
		{
			s->activateShaderProgram();  // ✅ Aktivuj
			s->updateUniform("spotlightCamera_bool", false);
			s->deactivateShaderProgram();  // ✅ Deaktivuj
		}
	}
}

void ShaderProgramManager::updateDirectionalLight()
{
	if (lm->getDirectionalLight())
	{
		DirectionalLight* light = lm->getDirectionalLight();

		for (ShaderProgram* s : shaderPrograms)
		{
			s->activateShaderProgram();  // ✅ Aktivuj

			s->updateUniform("directional_bool", true);
			s->updateUniform("directional.color", light->color);
			s->updateUniform("directional.direction", light->direction);

			s->deactivateShaderProgram();  // ✅ Deaktivuj
		}
	}
}

void ShaderProgramManager::updateLights()
{
	// Updatni všetky typy svetiel do všetkých shaderov
	updateAmbient();
	updatePointLights();
	updateSpotlights();
	updateSpotlightCamera();
	updateDirectionalLight();
}