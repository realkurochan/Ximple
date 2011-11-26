$.cleditor.buttons.quote = {
	name: "quote",
	image: "quote.png",
	title: "Quote",
	command: "inserthtml",
	buttonClick: quoteClick
};
 
function quoteClick(e, data){
	var html = '<blockquote>'+data.editor.selectedHTML()+'</blockquote>';
	data.editor.execCommand(data.command, html, null, data.button);
}
