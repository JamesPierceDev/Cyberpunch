#include "OnlineSendComponent.h"

OnlineSendComponent::OnlineSendComponent()
{
	
}

queue<string>* OnlineSendComponent::Send()
{
	return &m_commandsToSend;

}
