/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecvParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuusela <vkuusela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:31:54 by vkuusela          #+#    #+#             */
/*   Updated: 2025/08/22 12:58:20 by vkuusela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RecvParser.hpp"

/**
 *	Feed the recv() buffer into std::string buffer
 *	@param	read_buf	The buffer to read
 *	@param	len			Length of the buffer
 */
void	RecvParser::feed(const char *read_buf, size_t len)
{
	_buffer.append(read_buf, len);
	_normalizeNewLines();
	_parseBuffer();
}

/**
 *	Make all newlines conform to IRC protocol to make evals take less time
 */
void	RecvParser::_normalizeNewLines(void)
{
	std::string normalized;
	normalized.reserve(_buffer.size());
	for (size_t i = 0; i < _buffer.size(); ++i)
	{
		if (_buffer[i] == '\r')
		{
			if (i + 1 < _buffer.size() && _buffer[i + 1] == '\n')
			{
				normalized += "\r\n";
				++i;
			}
			else
				normalized += "\r\n";
		}
		else if (_buffer[i] == '\n')
			normalized += "\r\n";
		else
			normalized += _buffer[i];
	}
	_buffer = std::move(normalized);
}

/**
 *	Parse the internal buffer for commands.
 *	Newlines are \r\n as per IRC protocol.
 */
void	RecvParser::_parseBuffer(void)
{
	size_t pos;
	while ((pos = _buffer.find("\r\n")) != std::string::npos)
	{
		std::string line = _buffer.substr(0, pos);
		_buffer.erase(0, pos + 2);
		_handle(line);
	}
}

/**
 *	Handle the parsed line
 *	@param	msg	The line to handle
 */
void	RecvParser::_handle(const std::string &msg)
{
	auto hash = [](const std::string &msg)
	{
		if (msg.compare(0, 4, "NICK") == 0)
			return (NICK);
		if (msg.compare(0, 4, "USER") == 0)
			return (USER);
		if (msg.compare(0, 4, "JOIN") == 0)
			return (JOIN);
		if (msg.compare(0, 4, "PART") == 0)
			return (PART);
		if (msg.compare(0, 7, "PRIVMSG") == 0)
			return (PRIVMSG);
		if (msg.compare(0, 4, "KICK") == 0)
			return (KICK);
		if (msg.compare(0, 6, "INVITE") == 0)
			return (INVITE);
		if (msg.compare(0, 5, "TOPIC") == 0)
			return (TOPIC);
		if (msg.compare(0, 4, "QUIT") == 0)
			return (QUIT);
		if (msg.compare(0, 4, "MODE") == 0)
			return (MODE);
		return (ERROR);
	};
	std::cout << "Received: ";
	switch (hash(msg))
	{
		case NICK:
			std::cout << msg << std::endl;
			break ;
		case USER:
			std::cout << msg << std::endl;
			break ;
		case JOIN:
			std::cout << msg << std::endl;
			break ;
		case PART:
			std::cout << msg << std::endl;
			break ;
		case PRIVMSG:
			std::cout << msg << std::endl;
			break ;
		case KICK:
			std::cout << msg << std::endl;
			break ;
		case INVITE:
			std::cout << msg << std::endl;
			break ;
		case TOPIC:
			std::cout << msg << std::endl;
			break ;
		case QUIT:
			std::cout << msg << std::endl;
			break ;
		case MODE:
			std::cout << msg << std::endl;
			break ;
		default:
			std::cout << "bad/unsupported command" << std::endl;
	}
}
