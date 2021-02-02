DECLARE @text varchar(128) = 'test'
DECLARE @argv varchar(256) = N'/nt ' + @text

EXEC [PS_GameLog].dbo.[usp_PSM_Agent] @argv
