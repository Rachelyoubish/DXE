#pragma once

#include "Seacrest/Core.h"
#include "Seacrest/Events/Event.h"

namespace Seacrest {

	class SEACREST_API Layer
	{
	public:
		Layer( const std::string& name = "Layer" );
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}