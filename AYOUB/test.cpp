void			Response::deleteMethod(Request & request, RequestConfig & requestConf)
{
	ResponseHeader	head;
	(void)request;

	_response = "";
	if (pathIsFile(_path))
	{
		if (remove(_path.c_str()) == 0)
			_code = 204;
		else
			_code = 403;
	}
	else
		_code = 404;
	if (_code == 403 || _code == 404)
		_response = this->readHtml(_errorMap[_code]);
	_response = head.getHeader(_response.size(), _path, _code, _type, 
    requestConf.getContentLocation(), requestConf.getLang()) + "\r\n" + _response;
}