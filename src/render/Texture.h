#pragma once

namespace dd {

	class Texture final {
	public:
		void CreateWhite();
		void Bind(int unit) const;

	private:
		unsigned int m_id = 0;
	};

} // namespace dd
