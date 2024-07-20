#pragma once

class Bindable {
public:
	virtual void Bind() = 0;
	
	virtual void Unbind() = 0;
	
	virtual void Delete() = 0;
};
