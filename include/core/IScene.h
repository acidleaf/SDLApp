#ifndef __ISCENE_H__
#define __ISCENE_H__

class IStream;
class IScene {
protected:
	char _name[LABEL_SIZE];
public:
	IScene() { _name[0] = 0; }
	virtual ~IScene() {}

	virtual bool init() = 0;
	virtual void release() = 0;
	
	virtual void update() = 0;
	virtual void render() = 0;
	
	virtual bool handleEvents(const SDL_Event& e) = 0;

	virtual IStream* stream() = 0;

	virtual void renderMenu() {}

	virtual void name(const char* name) { SDL_strlcpy(_name, name, LABEL_SIZE); }
	virtual const char* name() const { return _name; }
};

#endif