/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecvParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuusela <vkuusela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:26:26 by vkuusela          #+#    #+#             */
/*   Updated: 2025/08/22 12:56:40 by vkuusela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <iostream>

enum cmd
{
	NICK,
	USER,
	JOIN,
	PART,
	PRIVMSG,
	KICK,
	INVITE,
	TOPIC,
	MODE,
	QUIT,
	ERROR
};

/**	
 * @class	RecvParser
 * @brief	A class for parsing data read by recv() for use by the client
 */
class	RecvParser
{
	public:
		void	feed(const char *read_buf, size_t len);

	private:
		std::string	_buffer;

		void	_normalizeNewLines(void);
		void	_parseBuffer(void);
		void	_handle(const std::string &msg);
};
