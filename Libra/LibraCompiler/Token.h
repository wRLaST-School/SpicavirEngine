#pragma once

namespace Libra {
	enum Token {
		tok_eof = -1,

		// commands
		tok_def = -2,
		tok_extern = -3,

		// primary
		tok_identifier = -4,
		tok_number = -5
	};
}
